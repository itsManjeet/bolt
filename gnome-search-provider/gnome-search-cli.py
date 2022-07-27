#!/usr/bin/python3

# Copyright 2017,2020  Sam Thursfield <sam@afuera.me.uk>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# GPL3 license.

# The relevant code in gnome-shell is:
#
#   https://git.gnome.org/browse/gnome-shell/tree/js/ui/remoteSearch.js
#   https://git.gnome.org/browse/gnome-shell/tree/js/ui/search.js
#
# There's a lot of UI code mixed in there.
#
# See also: <https://developer.gnome.org/SearchProvider/>


from gi.repository import Gio, GLib

import argparse
import configparser
import json
import logging
import os
import sys

log = logging.getLogger()

SEARCH_PROVIDERS_SETTINGS_SCHEMA = 'org.gnome.desktop.search-providers'

SEARCH_PROVIDER_DIR = '/usr/share/gnome-shell/search-providers/'


class RemoteSearchProvider():
    KEY_FILE_GROUP = 'Shell Search Provider'

    def __init__(self, bus_name: str, timeout_msec=2000):

        self.bus_name = bus_name
        self.object_path = '/' + bus_name.replace('.','/')

        self.desktop_id = bus_name + '.deskop'
        self.version = 2

        if self.version == 1:
            interface = 'org.gnome.Shell.SearchProvider'
        else:
            interface = 'org.gnome.Shell.SearchProvider2'

        bus = Gio.bus_get_sync(Gio.BusType.SESSION, None)
        self.proxy = Gio.DBusProxy.new_sync(
            bus, Gio.DBusProxyFlags.NONE, None, self.bus_name,
            self.object_path, interface, None)

        self.timeout = timeout_msec

    def __str__(self):
        return 'Search provider ' + self.desktop_id

    def get_id(self):
        return self.bus_name

    def get_desktop_id(self):
        return self.desktop_id

    def get_results_sync(self, terms, limit=None):
        '''Return results synchronously.

        This will usually block for a long time.

        '''
        log.debug("%s: Calling GetInitialResultSet()", self.get_desktop_id())
        args = GLib.Variant('(as)', (terms,))
        results = self.proxy.call_sync(
            'GetInitialResultSet', args, Gio.DBusCallFlags.NONE, self.timeout, None)[0]

        if len(results) > 0:
            if limit:
                results = results[:limit]
            args = GLib.Variant('(as)', (results,))
            log.debug("%s: Calling GetResultMetas()", self.get_desktop_id())
            result_metas = self.proxy.call_sync(
                'GetResultMetas', args, Gio.DBusCallFlags.NONE, self.timeout, None)[0]
            return result_metas
        else:
            return []

    def get_initial_result_set(self, terms, callback):
        '''Return initial results set.

        The callback() function will be called when the results are received.

        '''
        args = GLib.Variant('(as)', (terms,))
        log.debug("%s: Calling GetInitialResultSet()", self.get_desktop_id())
        self.proxy.call(
            'GetInitialResultSet', args, Gio.DBusCallFlags.NONE, self.timeout, None,
            callback)


def load_remote_search_providers():
    path = SEARCH_PROVIDER_DIR
    return [RemoteSearchProvider(os.path.join(path, filename))
            for filename in os.listdir(path)]


def argument_parser():
    parser = argparse.ArgumentParser(
        description="search all GNOME desktop search providers")
    parser.add_argument(
        '--debug', dest='debug', action='store_true',
        help="Enable detailed logging to stderr")
    parser.add_argument(
        '--limit', type=int, default=5,
        help="Maximum number of results to show from each provider")
    parser.add_argument(
        '--provider', metavar='ID',
        help="Limit to search provider with given id, e.g. `org.gnome.Clocks`."),
    parser.add_argument(
        'terms', metavar='terms', nargs='+', type=str,
        help='Terms to search for')
    return parser


def main():
    args = argument_parser().parse_args()

    if args.debug:
        logging.basicConfig(stream=sys.stderr, level=logging.DEBUG)

    provider = RemoteSearchProvider('dev.rlxos.Bolt.SearchProvider')

    logging.info("Searching %s" % provider)
    # FIXME: this should be cleverly async, like
    # a generator. For now it's not.
    try:
        results = list(provider.get_results_sync(args.terms, args.limit))

        def filter_results(r):
            if 'icon-data' in r:
                r['icon-data'] = "..."
            return r

        if len(results) > 0:
            json.dump({
                'provider': provider.get_id(),
                'results': list(map(filter_results, results)),
            }, indent=4, fp=sys.stdout)
    except GLib.Error as e:
        sys.stderr.write(f"Error contacting {provider}: {e}\n")


try:
    main()
except RuntimeError as e:
    sys.stderr.write(f"{e}\n")
    sys.exit(1)
