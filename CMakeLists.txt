cmake_minimum_required(VERSION 3.14)
project(puttyalt C)

set(CMAKE_C_STANDARD 99)

include(cmake/setup.cmake)

add_library(puttyalt STATIC
  puttyalt_portable.c
  puttyalt_reconnect.c
  puttyalt_geometry.c
  puttyalt_sessions.c
  puttyalt_snippets.c
  puttyalt_health.c
  puttyalt_logrotate.c
  puttyalt_cliphistory.c
  puttyalt_triggers.c
  puttyalt_broadcast.c
  puttyalt_keymanager.c
  puttyalt_termsearch.c
  puttyalt_envindicator.c
  puttyalt_quickconnect.c
  puttyalt_tabs.c
  puttyalt_sftppanel.c
  puttyalt_credstore.c
  puttyalt_theme.c
  puttyalt_import.c
  puttyalt_shortcuts.c
  puttyalt_notify.c
  puttyalt_proxydetect.c
  puttyalt_transfer.c
  puttyalt_latency.c
  puttyalt_cmdhistory.c
  puttyalt_statusbar.c
  puttyalt_timeout.c
  puttyalt_aliases.c
  puttyalt_pinning.c
  puttyalt_jumphost.c
  puttyalt_splitview.c
  puttyalt_macro.c
  puttyalt_loggingv2.c
  puttyalt_sesslock.c
  puttyalt_bandwidth.c
  puttyalt_zmodem.c
  puttyalt_autocomplete.c
  puttyalt_gesture.c
  puttyalt_workspace.c
  puttyalt_tunnel.c
  puttyalt_serial.c
  puttyalt_findreplace.c
  puttyalt_tags.c
  puttyalt_export2.c
  puttyalt_connpool.c
  puttyalt_urlhandler.c
  puttyalt_scp.c
  puttyalt_netdiag.c
  puttyalt_template.c
  puttyalt_colorpicker.c
  puttyalt_fontmgr.c
  puttyalt_startup.c
  puttyalt_layout.c
  puttyalt_migrate.c
  puttyalt_plugin.c
  puttyalt_eventbus.c
  puttyalt_ipc.c
  puttyalt_autoupdate.c
  puttyalt_bookmarks.c
  puttyalt_sharedclip.c
  puttyalt_keepalive.c
  puttyalt_bellv2.c
  puttyalt_agentfwd2.c
  puttyalt_x11fwd.c
  puttyalt_scheduler.c
  puttyalt_telnet.c
  puttyalt_perftimer.c
  puttyalt_mempool.c
  puttyalt_strbuf.c
  puttyalt_ini.c
  puttyalt_triggersv2.c
  puttyalt_keygen.c
  puttyalt_hostverify.c
  puttyalt_imgproto.c
  puttyalt_stats.c
  puttyalt_proxychain.c
  puttyalt_palette.c
  puttyalt_scrollcomp.c
  puttyalt_base64.c
  puttyalt_escparse.c
  puttyalt_validate.c
  puttyalt_sound.c
  puttyalt_gui.c
  puttyalt_dialogs.c
  puttyalt_tray.c
  puttyalt_scriptmgr.c
  puttyalt_connprof.c
  puttyalt_monitor.c
  puttyalt_mux.c
  puttyalt_sshagent.c
  puttyalt_clipmgr.c
  puttyalt_logengine.c
  puttyalt_codesign.c
  puttyalt_sessgroup.c
  puttyalt_renderer.c
  puttyalt_sshconfig.c
  puttyalt_expect.c
  puttyalt_recorder.c
  puttyalt_searchhl.c
  puttyalt_notifcenter.c
  puttyalt_quicklaunch.c
  puttyalt_unicode.c
  puttyalt_dashboard.c
  puttyalt_hexview.c
  puttyalt_fingerprint.c
  puttyalt_portfwd.c
  puttyalt_completion.c
  puttyalt_accessibility.c
  puttyalt_ratelimit.c
  puttyalt_chatlog.c
  puttyalt_envmgr.c
  puttyalt_sessreplay.c
  puttyalt_filebrowser.c
  puttyalt_inputfilter.c
  puttyalt_debugcon.c
  puttyalt_clipsync.c
  puttyalt_themes.c
  puttyalt_keybind.c
  puttyalt_sftpmgr.c
  puttyalt_bandwidth.c
  puttyalt_proxy.c
  puttyalt_sessnotes.c
  puttyalt_protoanalyze.c
  puttyalt_hotkeymgr.c
  puttyalt_colorscheme.c
  puttyalt_autobackup.c
  puttyalt_netscan.c
  puttyalt_termfx.c
  puttyalt_locale.c
  puttyalt_tunnelmon.c
  puttyalt_cmdpalette.c
  puttyalt_perfmon.c
  puttyalt_layout.c
  puttyalt_macro.c
  puttyalt_terminal_conf.c
  puttyalt_updater.c
  puttyalt_statusbar.c
)

target_include_directories(puttyalt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(UNIX)
  target_link_libraries(puttyalt PUBLIC dl m)
endif()

# GUI executable
add_executable(puttyalt-gui WIN32 main.c)
target_link_libraries(puttyalt-gui PRIVATE puttyalt)

if(WIN32)
  target_link_libraries(puttyalt-gui PRIVATE comctl32 shell32 comdlg32)
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/puttyalt.rc")
    target_sources(puttyalt-gui PRIVATE puttyalt.rc)
  endif()

  # Release build: optimize, strip debug info, enable security features
  # These flags significantly reduce AV false positives from MinGW builds
  target_compile_options(puttyalt-gui PRIVATE -O2 -DNDEBUG)
  target_compile_options(puttyalt PRIVATE -O2 -DNDEBUG)

  # Strip all symbols and debug sections from the final executable
  target_link_options(puttyalt-gui PRIVATE -s -Wl,--gc-sections)

  # Enable ASLR (high-entropy) and DEP
  target_link_options(puttyalt-gui PRIVATE
    -Wl,--dynamicbase
    -Wl,--high-entropy-va
    -Wl,--nxcompat
  )

  set_target_properties(puttyalt-gui PROPERTIES
    OUTPUT_NAME "puttyalt"
    WIN32_EXECUTABLE TRUE
  )
else()
  set_target_properties(puttyalt-gui PROPERTIES OUTPUT_NAME "puttyalt")
endif()

# v1.1.0 modules
list(APPEND PUTTYALT_SOURCES
    puttyalt_termemu.c
    puttyalt_selection.c
    puttyalt_ringbuf.c
    puttyalt_ansiparse.c
    puttyalt_colormgr.c
    puttyalt_fontchain.c
    puttyalt_urldetect.c
    puttyalt_ctxmenu.c
    puttyalt_quickbar.c
    puttyalt_favorites.c
    puttyalt_healthchk.c
    puttyalt_autoreconn.c
    puttyalt_jumpchain.c
    puttyalt_uilayer.c
    puttyalt_tooltip.c
    puttyalt_dockpanel.c
    puttyalt_cfgvalid.c
    puttyalt_wizard.c
    puttyalt_sessreplay.c
    puttyalt_schedit.c
    puttyalt_bellctl.c
)

# v1.2.0 UX & Usability modules
list(APPEND PUTTYALT_SOURCES
    puttyalt_smartpaste.c
    puttyalt_sesssearch.c
    puttyalt_passgen.c
    puttyalt_dragdrop.c
    puttyalt_snippetlib.c
    puttyalt_sessnote.c
    puttyalt_presets.c
    puttyalt_notifqueue.c
    puttyalt_searchoverlay.c
    puttyalt_sshkeymgr.c
    puttyalt_autocomplete_v2.c
    puttyalt_conntimeout.c
    puttyalt_multiclip.c
    puttyalt_envview.c
    puttyalt_quickcmd.c
)

# v1.3.0 Workflow & Productivity modules
list(APPEND PUTTYALT_SOURCES
    puttyalt_sessgroup_v2.c
    puttyalt_cmdhist_v2.c
    puttyalt_sftpsync.c
    puttyalt_outputcap.c
    puttyalt_conndash.c
    puttyalt_regtrig.c
    puttyalt_fileedit.c
    puttyalt_cronview.c
    puttyalt_smartresize.c
    puttyalt_tabcolors.c
    puttyalt_sshexport.c
)

# v1.3.1 patches
list(APPEND PUTTYALT_SOURCES
    puttyalt_osdetect.c
    puttyalt_speedtest.c
    puttyalt_tabpreview.c
    puttyalt_sessdiff.c
    puttyalt_inputrecord.c
    puttyalt_localecho.c
    puttyalt_retrypolicy.c
    puttyalt_timestamp.c
)
