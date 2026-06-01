cmake_minimum_required(VERSION 3.14)
project(puttyalt C)

set(CMAKE_C_STANDARD 99)

include(cmake/setup.cmake)

add_library(puttyalt STATIC
  src/core/puttyalt_portable.c
  src/network/puttyalt_reconnect.c
  src/config/puttyalt_geometry.c
  src/session/puttyalt_sessions.c
  src/editor/puttyalt_snippets.c
  src/tools/puttyalt_health.c
  src/tools/puttyalt_logrotate.c
  src/editor/puttyalt_cliphistory.c
  src/automation/puttyalt_triggers.c
  src/tools/puttyalt_broadcast.c
  src/security/puttyalt_keymanager.c
  src/terminal/puttyalt_termsearch.c
  src/ui/puttyalt_envindicator.c
  src/ui/puttyalt_quickconnect.c
  src/ui/puttyalt_tabs.c
  src/tools/puttyalt_sftppanel.c
  src/security/puttyalt_credstore.c
  src/tools/puttyalt_import.c
  src/tools/puttyalt_shortcuts.c
  src/tools/puttyalt_notify.c
  src/network/puttyalt_proxydetect.c
  src/tools/puttyalt_transfer.c
  src/network/puttyalt_latency.c
  src/tools/puttyalt_cmdhistory.c
  src/ui/puttyalt_statusbar.c
  src/config/puttyalt_timeout.c
  src/tools/puttyalt_aliases.c
  src/ui/puttyalt_pinning.c
  src/network/puttyalt_jumphost.c
  src/ui/puttyalt_splitview.c
  src/automation/puttyalt_macro.c
  src/tools/puttyalt_loggingv2.c
  src/session/puttyalt_sesslock.c
  src/network/puttyalt_bandwidth.c
  src/tools/puttyalt_zmodem.c
  src/tools/puttyalt_autocomplete.c
  src/tools/puttyalt_gesture.c
  src/tools/puttyalt_workspace.c
  src/network/puttyalt_tunnel.c
  src/tools/puttyalt_serial.c
  src/tools/puttyalt_findreplace.c
  src/tools/puttyalt_export2.c
  src/network/puttyalt_connpool.c
  src/tools/puttyalt_urlhandler.c
  src/tools/puttyalt_scp.c
  src/network/puttyalt_netdiag.c
  src/ui/puttyalt_colorpicker.c
  src/ui/puttyalt_fontmgr.c
  src/core/puttyalt_startup.c
  src/core/puttyalt_layout.c
  src/core/puttyalt_migrate.c
  src/core/puttyalt_plugin.c
  src/core/puttyalt_eventbus.c
  src/core/puttyalt_ipc.c
  src/tools/puttyalt_autoupdate.c
  src/tools/puttyalt_bookmarks.c
  src/editor/puttyalt_sharedclip.c
  src/network/puttyalt_keepalive.c
  src/terminal/puttyalt_bellv2.c
  src/network/puttyalt_agentfwd2.c
  src/network/puttyalt_x11fwd.c
  src/automation/puttyalt_scheduler.c
  src/network/puttyalt_telnet.c
  src/tools/puttyalt_perftimer.c
  src/tools/puttyalt_mempool.c
  src/tools/puttyalt_strbuf.c
  src/core/puttyalt_ini.c
  src/security/puttyalt_keygen.c
  src/security/puttyalt_hostverify.c
  src/core/puttyalt_imgproto.c
  src/tools/puttyalt_stats.c
  src/network/puttyalt_proxychain.c
  src/core/puttyalt_palette.c
  src/ui/puttyalt_scrollcomp.c
  src/tools/puttyalt_base64.c
  src/terminal/puttyalt_escparse.c
  src/config/puttyalt_validate.c
  src/core/puttyalt_sound.c
  src/core/puttyalt_gui.c
  src/core/puttyalt_dialogs.c
  src/core/puttyalt_tray.c
  src/automation/puttyalt_scriptmgr.c
  src/network/puttyalt_connprof.c
  src/network/puttyalt_monitor.c
  src/network/puttyalt_mux.c
  src/security/puttyalt_sshagent.c
  src/editor/puttyalt_clipmgr.c
  src/tools/puttyalt_logengine.c
  src/security/puttyalt_codesign.c
  src/session/puttyalt_sessgroup.c
  src/core/puttyalt_renderer.c
  src/network/puttyalt_sshconfig.c
  src/automation/puttyalt_expect.c
  src/tools/puttyalt_recorder.c
  src/ui/puttyalt_searchhl.c
  src/tools/puttyalt_notifcenter.c
  src/ui/puttyalt_quicklaunch.c
  src/core/puttyalt_unicode.c
  src/core/puttyalt_dashboard.c
  src/tools/puttyalt_hexview.c
  src/security/puttyalt_fingerprint.c
  src/network/puttyalt_portfwd.c
  src/tools/puttyalt_completion.c
  src/ui/puttyalt_accessibility.c
  src/security/puttyalt_ratelimit.c
  src/tools/puttyalt_chatlog.c
  src/tools/puttyalt_envmgr.c
  src/session/puttyalt_sessreplay.c
  src/tools/puttyalt_filebrowser.c
  src/security/puttyalt_inputfilter.c
  src/tools/puttyalt_debugcon.c
  src/editor/puttyalt_clipsync.c
  src/core/puttyalt_themes.c
  src/ui/puttyalt_keybind.c
  src/tools/puttyalt_sftpmgr.c
  src/network/puttyalt_proxy.c
  src/session/puttyalt_sessnotes.c
  src/network/puttyalt_protoanalyze.c
  src/tools/puttyalt_hotkeymgr.c
  src/ui/puttyalt_colorscheme.c
  src/tools/puttyalt_autobackup.c
  src/network/puttyalt_netscan.c
  src/terminal/puttyalt_termfx.c
  src/core/puttyalt_locale.c
  src/network/puttyalt_tunnelmon.c
  src/tools/puttyalt_perfmon.c
  src/terminal/puttyalt_terminal_conf.c
  src/tools/puttyalt_updater.c
)

target_include_directories(puttyalt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(UNIX)
  target_link_libraries(puttyalt PUBLIC dl m)
endif()

# GUI executable
add_executable(puttyalt-gui WIN32 main.c)
target_link_libraries(puttyalt-gui PRIVATE puttyalt)

if(WIN32)
  target_link_libraries(puttyalt-gui PRIVATE comctl32 shell32 comdlg32 dwmapi)
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
    src/terminal/puttyalt_termemu.c
    src/terminal/puttyalt_selection.c
    src/tools/puttyalt_ringbuf.c
    src/terminal/puttyalt_ansiparse.c
    src/ui/puttyalt_colormgr.c
    src/config/puttyalt_fontchain.c
    src/tools/puttyalt_urldetect.c
    src/core/puttyalt_ctxmenu.c
    src/ui/puttyalt_quickbar.c
    src/ui/puttyalt_favorites.c
    src/network/puttyalt_autoreconn.c
    src/network/puttyalt_jumpchain.c
    src/core/puttyalt_uilayer.c
    src/ui/puttyalt_tooltip.c
    src/ui/puttyalt_dockpanel.c
    src/config/puttyalt_cfgvalid.c
    src/core/puttyalt_wizard.c
    src/config/puttyalt_schedit.c
)

# v1.2.0 UX & Usability modules
list(APPEND PUTTYALT_SOURCES
    src/tools/puttyalt_smartpaste.c
    src/session/puttyalt_sesssearch.c
    src/security/puttyalt_passgen.c
    src/ui/puttyalt_dragdrop.c
    src/editor/puttyalt_snippetlib.c
    src/session/puttyalt_sessnote.c
    src/config/puttyalt_presets.c
    src/tools/puttyalt_notifqueue.c
    src/ui/puttyalt_searchoverlay.c
    src/security/puttyalt_sshkeymgr.c
    src/tools/puttyalt_autocomplete_v2.c
    src/network/puttyalt_conntimeout.c
    src/editor/puttyalt_multiclip.c
    src/tools/puttyalt_envview.c
    src/tools/puttyalt_quickcmd.c
)

# v1.3.0 Workflow & Productivity modules
list(APPEND PUTTYALT_SOURCES
    src/session/puttyalt_sessgroup_v2.c
    src/tools/puttyalt_cmdhist_v2.c
    src/tools/puttyalt_sftpsync.c
    src/tools/puttyalt_outputcap.c
    src/network/puttyalt_conndash.c
    src/automation/puttyalt_regtrig.c
    src/editor/puttyalt_fileedit.c
    src/tools/puttyalt_cronview.c
    src/ui/puttyalt_smartresize.c
    src/ui/puttyalt_tabcolors.c
    src/config/puttyalt_sshexport.c
)

# v1.3.1 patches
list(APPEND PUTTYALT_SOURCES
    src/tools/puttyalt_osdetect.c
    src/network/puttyalt_speedtest.c
    src/ui/puttyalt_tabpreview.c
    src/session/puttyalt_sessdiff.c
    src/tools/puttyalt_inputrecord.c
    src/terminal/puttyalt_localecho.c
    src/network/puttyalt_retrypolicy.c
    src/tools/puttyalt_timestamp.c
)

# v2.0.0 modules
list(APPEND PUTTYALT_SOURCES
    src/editor/puttyalt_snipsync.c
    src/security/puttyalt_audit.c
)

# v2.0.0 extended modules
list(APPEND PUTTYALT_SOURCES
    src/ui/puttyalt_transitions.c
    src/ui/puttyalt_cmdpal.c
    src/session/puttyalt_sessshare.c
    src/security/puttyalt_roles.c
    src/config/puttyalt_cfgsync.c
    src/automation/puttyalt_luaengine.c
    src/automation/puttyalt_webhooks.c
    src/automation/puttyalt_pipeline.c
    src/automation/puttyalt_restapi.c
    src/ui/puttyalt_taboverflow.c
    src/ui/puttyalt_sidebarresize.c
    src/ui/puttyalt_cursor.c
    src/core/puttyalt_paths.c
    src/ui/puttyalt_focusring.c
    src/ui/puttyalt_toast.c
    src/ui/puttyalt_scrollstate.c
    src/ui/puttyalt_tabnav.c
    src/session/puttyalt_sessorder.c
    src/tools/puttyalt_uptime.c
)

# v2.0.0 phase 3 modules
list(APPEND PUTTYALT_SOURCES
    src/config/puttyalt_activity.c
    src/config/puttyalt_templates.c
    src/security/puttyalt_approval.c
    src/ui/puttyalt_shortcutoverlay.c
    src/ui/puttyalt_themeswitch.c
    src/session/puttyalt_sesstags.c
    src/editor/puttyalt_clipboard_history.c
    src/ui/puttyalt_breadcrumb.c
    src/ui/puttyalt_minimap.c
    src/ui/puttyalt_fontpicker.c
    src/ui/puttyalt_opacity_ctrl.c
    src/tools/puttyalt_idle_detect.c
    src/session/puttyalt_session_lock.c
    src/network/puttyalt_reconnect_v2.c
    src/network/puttyalt_bandwidth_limit.c
    src/security/puttyalt_ssh_agent_v2.c
    src/network/puttyalt_proxy_detect.c
    src/network/puttyalt_dns_cache.c
    src/network/puttyalt_keepalive_v2.c
    src/session/puttyalt_session_notes_v2.c
    src/ui/puttyalt_quick_actions.c
    src/editor/puttyalt_sniplib.c
)

# v2.1.0 split panes, GPU render, inline images, plugin API v2, cloud sync, AI suggestions
list(APPEND PUTTYALT_SOURCES
    src/ui/puttyalt_splitpane.c
    src/ui/puttyalt_panenav.c
    src/ui/puttyalt_paneresize.c
    src/core/puttyalt_d2dbackend.c
    src/core/puttyalt_rendercache.c
    src/core/puttyalt_dirtyrect.c
    src/core/puttyalt_framepace.c
    src/automation/puttyalt_pmanifest.c
    src/security/puttyalt_pgrant.c
    src/automation/puttyalt_msgbus.c
    src/automation/puttyalt_pluglife.c
    src/automation/puttyalt_plugreg.c
    src/automation/puttyalt_hookdispatch.c
    src/security/puttyalt_syncenvelope.c
    src/session/puttyalt_syncconflict.c
    src/automation/puttyalt_cmdsuggest.c
    src/automation/puttyalt_cmdfreqmodel.c
    src/tools/puttyalt_histfuzzy.c
    src/tools/puttyalt_sugcache.c
    src/terminal/puttyalt_itermimg.c
    src/terminal/puttyalt_kittygfx.c
    src/terminal/puttyalt_imgcell.c
    src/terminal/puttyalt_imgscale.c
    src/terminal/puttyalt_gfxbudget.c
    src/terminal/puttyalt_oscroute.c
    src/ui/puttyalt_panelsnap.c
    src/config/puttyalt_layoutstore.c
    src/session/puttyalt_wsmanager.c
    src/terminal/puttyalt_panecast.c
    src/ui/puttyalt_panezoom.c
    src/session/puttyalt_bootrestore.c
    src/ui/puttyalt_launchentries.c
)

# v2.2.0 Vulkan/GPU, collaboration, performance, advanced terminal, cloud transport
list(APPEND PUTTYALT_SOURCES
    src/network/puttyalt_cloudsync.c
    src/network/puttyalt_lathist.c
    src/network/puttyalt_readsizer.c
    src/network/puttyalt_writebatch.c
    src/network/puttyalt_wsframe.c
    src/security/puttyalt_auditchain.c
    src/security/puttyalt_credscope.c
    src/security/puttyalt_keyrotate.c
    src/security/puttyalt_sharetier.c
    src/security/puttyalt_tofupin.c
    src/security/puttyalt_vaultring.c
    src/session/puttyalt_coedit.c
    src/session/puttyalt_optransform.c
    src/session/puttyalt_presence.c
    src/terminal/puttyalt_bidirun.c
    src/terminal/puttyalt_blockrect.c
    src/terminal/puttyalt_cellslab.c
    src/terminal/puttyalt_cmdtrack.c
    src/terminal/puttyalt_damageregion.c
    src/terminal/puttyalt_glyphatlas.c
    src/terminal/puttyalt_graphwidth.c
    src/terminal/puttyalt_hyperlinx.c
    src/terminal/puttyalt_inputcoal.c
    src/terminal/puttyalt_ligshape.c
    src/terminal/puttyalt_lineundo.c
    src/terminal/puttyalt_promptmark.c
    src/terminal/puttyalt_reflowsched.c
    src/terminal/puttyalt_rendercmdq.c
    src/terminal/puttyalt_sbarena.c
    src/terminal/puttyalt_subpixelpos.c
    src/terminal/puttyalt_tabstops.c
    src/terminal/puttyalt_vkbackend.c
    src/terminal/puttyalt_vsyncctl.c
    src/tools/puttyalt_asciicast.c
    src/ui/puttyalt_annotate.c
    src/ui/puttyalt_scrubber.c
)

# v2.3.0 mobile/touch, accessibility, modern protocols (mosh/SSH3), local analytics
list(APPEND PUTTYALT_SOURCES
    src/automation/puttyalt_condtrig.c
    src/automation/puttyalt_scriptvar.c
    src/automation/puttyalt_snippetexp.c
    src/automation/puttyalt_taskqueue.c
    src/config/puttyalt_kvstore.c
    src/config/puttyalt_migstep.c
    src/config/puttyalt_profilexport.c
    src/network/puttyalt_connretry.c
    src/network/puttyalt_happyeye.c
    src/network/puttyalt_mosh.c
    src/network/puttyalt_proxyauto.c
    src/network/puttyalt_sockstat.c
    src/network/puttyalt_ssh3.c
    src/security/puttyalt_keyfmt.c
    src/security/puttyalt_pwstrength.c
    src/session/puttyalt_sesscolor.c
    src/session/puttyalt_sessmru.c
    src/session/puttyalt_sesstemplate.c
    src/terminal/puttyalt_altscreen.c
    src/terminal/puttyalt_charwidth.c
    src/terminal/puttyalt_palette256.c
    src/terminal/puttyalt_reflowcache.c
    src/tools/puttyalt_byteunit.c
    src/tools/puttyalt_clipfmt.c
    src/tools/puttyalt_durparse.c
    src/tools/puttyalt_grepfilter.c
    src/tools/puttyalt_jsonmini.c
    src/tools/puttyalt_localstats.c
    src/ui/puttyalt_breadtrail.c
    src/ui/puttyalt_hicontrast.c
    src/ui/puttyalt_oskbd.c
    src/ui/puttyalt_srqueue.c
    src/ui/puttyalt_toastqueue.c
    src/ui/puttyalt_touchgest.c
    src/ui/puttyalt_vscroll.c
    src/ui/puttyalt_zoompan.c
)
