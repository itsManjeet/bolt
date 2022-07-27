install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/gnome-search-provider.py
    PERMISSIONS OWNER_EXECUTE OWNER_READ OWNER_WRITE GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ
    DESTINATION lib/bolt
)

install(
    FILES dev.rlxos.Bolt.SearchProvider.desktop
    DESTINATION share/applications
)

install(
    FILES dev.rlxos.Bolt.SearchProvider.ini
    DESTINATION share/gnome-shell/search-providers
)

install(
    FILES dev.rlxos.Bolt.SearchProvider.service
    DESTINATION lib/systemd/user
)

install(
    FILES dev.rlxos.Bolt.SearchProvider.service.dbus
    DESTINATION share/dbus-1/services/
    RENAME dev.rlxos.Bolt.SearchProvider.service
)