#!/usr/bin/env python3

from http.client import HTTPConnection
from os import getenv
import socket
import sys
import dbus.service
from dbus.mainloop.glib import DBusGMainLoop
from gi.repository import GLib
import requests
from requests.adapters import HTTPAdapter
from urllib3 import HTTPConnectionPool

SEARCH_BUS_NAME = "org.gnome.Shell.SearchProvider2"
SBN = dict(dbus_interface=SEARCH_BUS_NAME)


class BoltConnection(HTTPConnection):
    def __init__(self):
        super().__init__("localhost")
    def connect(self):
        self.sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        self.sock.connect('/run/bolt.sock')

class BoltConnectionPool(HTTPConnectionPool):
    def __init__(self):
        super().__init__('localhost')
    
    def _new_conn(self):
        return BoltConnection()

class BoltAdapter(HTTPAdapter):
    def get_connection(self, url, proxies=None):
        return BoltConnectionPool()

class BoltSearchProvider(dbus.service.Object):
    BUS_NAME = "dev.rlxos.Bolt.SearchProvider"
    OBJECT_PATH = "/" + BUS_NAME.replace(".", "/")

    def __init__(self):
        self.session_bus = dbus.SessionBus()
        busname = dbus.service.BusName(self.BUS_NAME, bus=self.session_bus)
        dbus.service.Object.__init__(self, busname, self.OBJECT_PATH)
        self.session = requests.Session()
        self.session.mount("http://bolt/", BoltAdapter())
    

    @dbus.service.method(in_signature="sasu", **SBN)
    def ActivateResult(self, id, terms, timestamp):
        pass

    @dbus.service.method(in_signature="as", out_signature="as", **SBN)
    def GetInitialResultSet(self, terms):
        input_mesg = " ".join(terms)
        response = self.session.get(
            "http://bolt/predict", params={"input": input_mesg}
        )
        if response.json()['status']:
            return [response.json()["data"]]
        return []

    @dbus.service.method(in_signature="as", out_signature="aa{sv}", **SBN)
    def GetResultMetas(self, ids):
        sys.stderr.write(','.join(ids))
        resp = []
        for id in ids:
            resp.append({'id': 'bolt', 'name': id})
        return resp
    @dbus.service.method(in_signature='asas', out_signature='as', **SBN)
    def GetSubsearchResultSet(self, previous_results, new_terms):
        input_mesg = " ".join(new_terms)
        response = self.session.get(
            "http://bolt/predict", params={"input": input_mesg}
        )
        if response.json()['status']:
            return [response.json()["data"]]
        return []
    
    @dbus.service.method(in_signature="asu", terms="as", timestamp="u", **SBN)
    def LaunchSearch(self, terms, timestamp):
        pass

if __name__ == '__main__':
    print('starting service')
    DBusGMainLoop(set_as_default=True)
    BoltSearchProvider()
    GLib.MainLoop().run()
