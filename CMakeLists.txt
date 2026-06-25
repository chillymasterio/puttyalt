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

# v2.4.0 AI assist, connection multiplexing, cloud profiles, advanced SFTP
list(APPEND PUTTYALT_SOURCES
    src/automation/puttyalt_aicache.c
    src/automation/puttyalt_aiprompt.c
    src/automation/puttyalt_aiseq.c
    src/automation/puttyalt_cmdexplain.c
    src/automation/puttyalt_retryctx.c
    src/automation/puttyalt_workflowstep.c
    src/config/puttyalt_hotreload.c
    src/config/puttyalt_settingdiff.c
    src/config/puttyalt_themeval.c
    src/core/puttyalt_chanwin.c
    src/core/puttyalt_muxchan.c
    src/network/puttyalt_bwthrottle.c
    src/network/puttyalt_keyexch.c
    src/network/puttyalt_quicflow.c
    src/security/puttyalt_authmethod.c
    src/security/puttyalt_hostkeyrot.c
    src/session/puttyalt_cloudprofile.c
    src/session/puttyalt_sessgraph.c
    src/session/puttyalt_sessshare2.c
    src/terminal/puttyalt_csiparse.c
    src/terminal/puttyalt_oscclip.c
    src/terminal/puttyalt_oscprogress.c
    src/terminal/puttyalt_scrollregion.c
    src/tools/puttyalt_colordiff.c
    src/tools/puttyalt_diffhunk.c
    src/tools/puttyalt_logsearch.c
    src/tools/puttyalt_pathcomplete.c
    src/tools/puttyalt_ratecalc.c
    src/tools/puttyalt_sftpmirror.c
    src/tools/puttyalt_sftpqueue.c
    src/tools/puttyalt_sftpresume.c
    src/ui/puttyalt_cmdbar.c
    src/ui/puttyalt_splitlayout.c
    src/ui/puttyalt_statusseg.c
)

# v3.0.0 New Era: AI copilot/agent, zero-trust security, distributed sessions, GPU compositor
list(APPEND PUTTYALT_SOURCES
    src/automation/puttyalt_aiagent.c
    src/automation/puttyalt_copilot.c
    src/automation/puttyalt_eventstream.c
    src/automation/puttyalt_pipeline2.c
    src/automation/puttyalt_scenario.c
    src/config/puttyalt_featureflags.c
    src/config/puttyalt_settingsync.c
    src/network/puttyalt_connpool2.c
    src/network/puttyalt_dohresolve.c
    src/network/puttyalt_multipath.c
    src/network/puttyalt_sshmux.c
    src/security/puttyalt_certpin.c
    src/security/puttyalt_mfaflow.c
    src/security/puttyalt_vault2.c
    src/security/puttyalt_zerotrust.c
    src/session/puttyalt_distsess.c
    src/session/puttyalt_sesssync2.c
    src/session/puttyalt_workspace2.c
    src/terminal/puttyalt_compositor.c
    src/terminal/puttyalt_gpurender.c
    src/terminal/puttyalt_shaper.c
    src/terminal/puttyalt_termstate2.c
    src/tools/puttyalt_cmdrunner.c
    src/tools/puttyalt_obsv.c
    src/tools/puttyalt_recorder2.c
    src/tools/puttyalt_tracer.c
    src/ui/puttyalt_cmdpalette2.c
    src/ui/puttyalt_dashboard2.c
    src/ui/puttyalt_notifcenter2.c
    src/ui/puttyalt_themeengine.c
)

# v3.1.0 post-quantum crypto, WASM plugins, edge/IoT, Unicode 15.1, modern protocols
list(APPEND PUTTYALT_SOURCES
    src/automation/puttyalt_hookchain.c
    src/automation/puttyalt_jobsched.c
    src/automation/puttyalt_ratelimit2.c
    src/automation/puttyalt_retryqueue.c
    src/automation/puttyalt_wasmrt.c
    src/config/puttyalt_backupsched.c
    src/config/puttyalt_defaults2.c
    src/config/puttyalt_envprofile.c
    src/config/puttyalt_schemaval.c
    src/network/puttyalt_edgeconn.c
    src/network/puttyalt_natpunch.c
    src/network/puttyalt_proxyhop.c
    src/network/puttyalt_tlsinfo.c
    src/security/puttyalt_authlog.c
    src/security/puttyalt_keyderive.c
    src/security/puttyalt_pqkem.c
    src/session/puttyalt_quickconnect2.c
    src/session/puttyalt_sessfolder.c
    src/session/puttyalt_sessicon.c
    src/terminal/puttyalt_colorquant.c
    src/terminal/puttyalt_decmode.c
    src/terminal/puttyalt_kitty_kbd.c
    src/terminal/puttyalt_synchout.c
    src/terminal/puttyalt_unicode15.c
    src/tools/puttyalt_b64stream.c
    src/tools/puttyalt_clipsync2.c
    src/tools/puttyalt_filesync2.c
    src/tools/puttyalt_logexport.c
    src/tools/puttyalt_searchidx.c
    src/tools/puttyalt_wasmhost.c
    src/ui/puttyalt_minimap2.c
    src/ui/puttyalt_palettecmd.c
    src/ui/puttyalt_statusicon.c
    src/ui/puttyalt_tabscroll.c
)

# v3.2.0 enterprise compliance, advanced terminal graphics, smart networking, dev tooling
list(APPEND PUTTYALT_SOURCES
    src/automation/puttyalt_alertrule.c
    src/automation/puttyalt_cmdapproval.c
    src/automation/puttyalt_runbook.c
    src/automation/puttyalt_statemachine.c
    src/automation/puttyalt_webhook2.c
    src/config/puttyalt_keymap2.c
    src/config/puttyalt_policyfile.c
    src/config/puttyalt_rbac.c
    src/network/puttyalt_bwgraph.c
    src/network/puttyalt_dnssec.c
    src/network/puttyalt_geoip.c
    src/security/puttyalt_compliance.c
    src/security/puttyalt_idtoken.c
    src/security/puttyalt_secretscan.c
    src/security/puttyalt_sessrecord.c
    src/session/puttyalt_sessbatch.c
    src/session/puttyalt_sessquota.c
    src/session/puttyalt_sessvault.c
    src/terminal/puttyalt_graphemes.c
    src/terminal/puttyalt_sixel2.c
    src/terminal/puttyalt_titlestack.c
    src/terminal/puttyalt_underline.c
    src/tools/puttyalt_auditquery.c
    src/tools/puttyalt_csvparse.c
    src/tools/puttyalt_diffmerge.c
    src/tools/puttyalt_hexedit.c
    src/tools/puttyalt_metricspush.c
    src/tools/puttyalt_portscan.c
    src/tools/puttyalt_treeview.c
    src/ui/puttyalt_inputhint.c
    src/ui/puttyalt_tabgroup2.c
    src/ui/puttyalt_themepick2.c
)

# v3.3.0 AI/ML ops, session intelligence, modern transfer, polish
list(APPEND PUTTYALT_SOURCES
    src/automation/puttyalt_anomaly.c
    src/automation/puttyalt_cronexpr.c
    src/automation/puttyalt_pipefilter.c
    src/automation/puttyalt_predict.c
    src/automation/puttyalt_smartretry.c
    src/automation/puttyalt_workflow2.c
    src/config/puttyalt_profilemgr.c
    src/config/puttyalt_secretref.c
    src/config/puttyalt_settingui.c
    src/network/puttyalt_conngraph.c
    src/network/puttyalt_loadbalance.c
    src/network/puttyalt_rsync.c
    src/session/puttyalt_sessexport2.c
    src/session/puttyalt_sessscore.c
    src/session/puttyalt_sessshare3.c
    src/session/puttyalt_sesstimeline.c
    src/terminal/puttyalt_charset.c
    src/terminal/puttyalt_imgrender.c
    src/terminal/puttyalt_scrollmark.c
    src/terminal/puttyalt_smartcopy.c
    src/tools/puttyalt_archive.c
    src/tools/puttyalt_cmdsafety.c
    src/tools/puttyalt_exportcsv.c
    src/tools/puttyalt_jsonpath.c
    src/tools/puttyalt_logcluster.c
    src/tools/puttyalt_progressbar.c
    src/tools/puttyalt_snippetlib2.c
    src/ui/puttyalt_cmdhelp.c
    src/ui/puttyalt_paneborder.c
    src/ui/puttyalt_quickaction.c
    src/ui/puttyalt_searchpanel.c
    src/ui/puttyalt_tooltip2.c
)

# v3.4.0 SSH protocol depth, transport tuning, advanced UI, utilities
list(APPEND PUTTYALT_SOURCES
    src/automation/puttyalt_expect2.c
    src/automation/puttyalt_macroplay.c
    src/automation/puttyalt_schedule2.c
    src/automation/puttyalt_trigger3.c
    src/config/puttyalt_configdiff.c
    src/config/puttyalt_remember.c
    src/network/puttyalt_keepalive3.c
    src/network/puttyalt_portforward2.c
    src/network/puttyalt_socks5.c
    src/network/puttyalt_sshcompress.c
    src/network/puttyalt_sshrekey.c
    src/security/puttyalt_hostkey2.c
    src/session/puttyalt_sessfilter.c
    src/session/puttyalt_sessgroup3.c
    src/session/puttyalt_sessnote2.c
    src/session/puttyalt_sesstag2.c
    src/terminal/puttyalt_mouse2.c
    src/terminal/puttyalt_oscfont.c
    src/terminal/puttyalt_rectsel.c
    src/terminal/puttyalt_wrapstate.c
    src/tools/puttyalt_diffstat.c
    src/tools/puttyalt_filepick.c
    src/tools/puttyalt_iprange.c
    src/tools/puttyalt_sshfp.c
    src/tools/puttyalt_tarlist.c
    src/tools/puttyalt_textwrap.c
    src/tools/puttyalt_throughput.c
    src/tools/puttyalt_uuid.c
    src/tools/puttyalt_yamlparse.c
    src/ui/puttyalt_animcurve.c
    src/ui/puttyalt_colorwheel.c
    src/ui/puttyalt_contextbar.c
    src/ui/puttyalt_statusline2.c
)

# v3.5.0 terminal emulation depth, hardware auth, protocol parsers, UI primitives
list(APPEND PUTTYALT_SOURCES
    src/automation/puttyalt_chainrun.c
    src/automation/puttyalt_eventhook.c
    src/automation/puttyalt_luaapi.c
    src/automation/puttyalt_restendpoint.c
    src/config/puttyalt_inifile.c
    src/config/puttyalt_keyseq.c
    src/network/puttyalt_iface.c
    src/network/puttyalt_proxyssl.c
    src/network/puttyalt_sshtunnel.c
    src/security/puttyalt_fido2.c
    src/security/puttyalt_pkcs11.c
    src/session/puttyalt_recentconn.c
    src/session/puttyalt_sessstats.c
    src/terminal/puttyalt_apc.c
    src/terminal/puttyalt_attrs.c
    src/terminal/puttyalt_cursorshape.c
    src/terminal/puttyalt_dcs.c
    src/terminal/puttyalt_linedraw.c
    src/terminal/puttyalt_reflow2.c
    src/terminal/puttyalt_screenbuf.c
    src/terminal/puttyalt_tabstop.c
    src/terminal/puttyalt_vt220.c
    src/tools/puttyalt_glob.c
    src/tools/puttyalt_md5.c
    src/tools/puttyalt_scpv2.c
    src/tools/puttyalt_sftpattr.c
    src/tools/puttyalt_timefmt.c
    src/tools/puttyalt_wordcomplete.c
    src/ui/puttyalt_modaldialog.c
    src/ui/puttyalt_scrollbar.c
    src/ui/puttyalt_splitresize.c
    src/ui/puttyalt_tablist.c
)

# v3.6.0 editor primitives, search, data viz, integrations, polish
list(APPEND PUTTYALT_SOURCES
    src/automation/puttyalt_batchexec.c
    src/automation/puttyalt_pluginhost.c
    src/automation/puttyalt_scriptload.c
    src/automation/puttyalt_taskpool.c
    src/config/puttyalt_settingstore.c
    src/config/puttyalt_themeload.c
    src/editor/puttyalt_buffer.c
    src/editor/puttyalt_undostack.c
    src/network/puttyalt_bandwidthmon.c
    src/network/puttyalt_dnsresolve.c
    src/network/puttyalt_wsclient.c
    src/session/puttyalt_bookmark2.c
    src/session/puttyalt_sessmeta.c
    src/session/puttyalt_sessrestore2.c
    src/terminal/puttyalt_colorscheme2.c
    src/terminal/puttyalt_palette2.c
    src/terminal/puttyalt_scrollback2.c
    src/tools/puttyalt_bytefmt.c
    src/tools/puttyalt_clipformat.c
    src/tools/puttyalt_fuzzy2.c
    src/tools/puttyalt_hashverify.c
    src/tools/puttyalt_regex2.c
    src/tools/puttyalt_sparkline.c
    src/tools/puttyalt_tablefmt.c
    src/tools/puttyalt_validate2.c
    src/ui/puttyalt_breadcrumb2.c
    src/ui/puttyalt_gauge.c
    src/ui/puttyalt_keymapui.c
    src/ui/puttyalt_progressmodal.c
    src/ui/puttyalt_searchhl2.c
    src/ui/puttyalt_tooltip3.c
)

# v3.7.0 i18n/a11y, profiling, IPv6, advanced parsing, UI primitives
list(APPEND PUTTYALT_SOURCES
    src/automation/puttyalt_condvar.c
    src/automation/puttyalt_macroedit.c
    src/automation/puttyalt_scriptdebug.c
    src/config/puttyalt_export3.c
    src/config/puttyalt_i18n.c
    src/config/puttyalt_import2.c
    src/config/puttyalt_locale2.c
    src/editor/puttyalt_syntax.c
    src/network/puttyalt_connstate.c
    src/network/puttyalt_ipv6.c
    src/network/puttyalt_reconnect3.c
    src/security/puttyalt_audit2.c
    src/session/puttyalt_connhistory2.c
    src/session/puttyalt_sessgrid.c
    src/session/puttyalt_sesssearch2.c
    src/terminal/puttyalt_bidi2.c
    src/terminal/puttyalt_charcount.c
    src/terminal/puttyalt_escfilter.c
    src/terminal/puttyalt_oscparse2.c
    src/tools/puttyalt_cmdparse.c
    src/tools/puttyalt_jsonbuild.c
    src/tools/puttyalt_levenshtein.c
    src/tools/puttyalt_perfprof.c
    src/tools/puttyalt_ratecalc2.c
    src/tools/puttyalt_ringbuf2.c
    src/tools/puttyalt_throttle2.c
    src/ui/puttyalt_a11y2.c
    src/ui/puttyalt_notifytoast.c
    src/ui/puttyalt_radialmenu.c
    src/ui/puttyalt_splitlayout2.c
    src/ui/puttyalt_zoom2.c
)

# v3.8.0 crypto helpers (base32/TOTP/CRC32), UTF-8, UI widgets, protocol
list(APPEND PUTTYALT_SOURCES
    src/automation/puttyalt_actionqueue.c
    src/automation/puttyalt_replaymacro.c
    src/automation/puttyalt_schedrule.c
    src/automation/puttyalt_triggermatch.c
    src/config/puttyalt_cfgmigrate2.c
    src/config/puttyalt_keybindmap.c
    src/config/puttyalt_preset2.c
    src/network/puttyalt_bandwidthlimit2.c
    src/network/puttyalt_packetlog.c
    src/network/puttyalt_proto2.c
    src/network/puttyalt_urlparse.c
    src/security/puttyalt_keystore.c
    src/security/puttyalt_totp.c
    src/session/puttyalt_sessclone2.c
    src/session/puttyalt_sesspin.c
    src/terminal/puttyalt_ansi256.c
    src/terminal/puttyalt_cursorblink.c
    src/terminal/puttyalt_selection2.c
    src/terminal/puttyalt_wrapindent.c
    src/tools/puttyalt_base32.c
    src/tools/puttyalt_crc32.c
    src/tools/puttyalt_diffview.c
    src/tools/puttyalt_filetype.c
    src/tools/puttyalt_hexdump2.c
    src/tools/puttyalt_mempool2.c
    src/tools/puttyalt_quote.c
    src/tools/puttyalt_statemgr.c
    src/tools/puttyalt_unicode2.c
    src/ui/puttyalt_dropdown.c
    src/ui/puttyalt_listview.c
    src/ui/puttyalt_spinner.c
)

# v3.9.0 JSON/text diff, validation, layout, caching, protocol
list(APPEND PUTTYALT_SOURCES
    src/automation/puttyalt_conditionchk.c
    src/automation/puttyalt_eventbus2.c
    src/automation/puttyalt_jobqueue2.c
    src/automation/puttyalt_pipeexec.c
    src/automation/puttyalt_taskdep.c
    src/config/puttyalt_optparse.c
    src/config/puttyalt_themevar.c
    src/config/puttyalt_validate3.c
    src/network/puttyalt_addrpool.c
    src/network/puttyalt_connpool3.c
    src/network/puttyalt_dnscache2.c
    src/network/puttyalt_tlsverify.c
    src/security/puttyalt_pwpolicy.c
    src/session/puttyalt_sessbookmark.c
    src/session/puttyalt_sesstemplate2.c
    src/terminal/puttyalt_charmap.c
    src/terminal/puttyalt_inputmode.c
    src/terminal/puttyalt_scrollregion2.c
    src/terminal/puttyalt_termresize.c
    src/tools/puttyalt_cache2.c
    src/tools/puttyalt_jsonpatch.c
    src/tools/puttyalt_logfmt.c
    src/tools/puttyalt_pathutil.c
    src/tools/puttyalt_retrytimer.c
    src/tools/puttyalt_strutil.c
    src/tools/puttyalt_textdiff.c
    src/tools/puttyalt_timeparse.c
    src/ui/puttyalt_grid.c
    src/ui/puttyalt_menubar.c
    src/ui/puttyalt_panel.c
    src/ui/puttyalt_progressring.c
    src/ui/puttyalt_statuswidget.c
)

# v3.10.0 data structures, codecs, automation safety, UI primitives
list(APPEND PUTTYALT_SOURCES
    src/automation/puttyalt_flowctl.c
    src/automation/puttyalt_loopguard.c
    src/automation/puttyalt_retrpolicy.c
    src/automation/puttyalt_scriptctx.c
    src/automation/puttyalt_workflowval.c
    src/config/puttyalt_envexpand.c
    src/config/puttyalt_profilediff.c
    src/config/puttyalt_settingvalidate.c
    src/network/puttyalt_dohclient.c
    src/network/puttyalt_keepalive4.c
    src/network/puttyalt_mtu2.c
    src/network/puttyalt_proxyauth.c
    src/security/puttyalt_certexpiry.c
    src/security/puttyalt_keyfp2.c
    src/session/puttyalt_sessgroup4.c
    src/session/puttyalt_sesshistory.c
    src/session/puttyalt_sessmru2.c
    src/terminal/puttyalt_celldiff.c
    src/terminal/puttyalt_oscstatus.c
    src/terminal/puttyalt_scrollanim.c
    src/terminal/puttyalt_termmode.c
    src/tools/puttyalt_base64url.c
    src/tools/puttyalt_bitset.c
    src/tools/puttyalt_csvwrite.c
    src/tools/puttyalt_humanize.c
    src/tools/puttyalt_numfmt.c
    src/tools/puttyalt_ringqueue.c
    src/tools/puttyalt_tokenizer.c
    src/ui/puttyalt_splitbar.c
    src/ui/puttyalt_tabcontext.c
    src/ui/puttyalt_toastmgr.c
    src/ui/puttyalt_treelist.c
)

# v3.11.0 text processing, parsing, codecs, terminal & UI utilities
list(APPEND PUTTYALT_SOURCES
    src/automation/puttyalt_cooldown.c
    src/automation/puttyalt_ratelimit2.c
    src/automation/puttyalt_tmplengine.c
    src/config/puttyalt_glob2.c
    src/config/puttyalt_iniwrite.c
    src/config/puttyalt_keymapparse.c
    src/network/puttyalt_cidr4.c
    src/network/puttyalt_macaddr.c
    src/network/puttyalt_portrange.c
    src/network/puttyalt_urlencode.c
    src/security/puttyalt_hostkeyfmt.c
    src/security/puttyalt_pwentropy.c
    src/session/puttyalt_exporthtml.c
    src/session/puttyalt_tagindex.c
    src/terminal/puttyalt_charwidth.c
    src/terminal/puttyalt_cursorsave.c
    src/terminal/puttyalt_sgrparse.c
    src/terminal/puttyalt_tabstops.c
    src/tools/puttyalt_base32hex.c
    src/tools/puttyalt_checksum8.c
    src/tools/puttyalt_columnize.c
    src/tools/puttyalt_csvparse.c
    src/tools/puttyalt_levenratio.c
    src/tools/puttyalt_rle.c
    src/tools/puttyalt_shquote.c
    src/tools/puttyalt_slugify.c
    src/tools/puttyalt_strtrim2.c
    src/tools/puttyalt_timefmt.c
    src/tools/puttyalt_wordwrap2.c
    src/ui/puttyalt_progressbar.c
    src/ui/puttyalt_spinner.c
)

# v3.12.0 numeric, data structures, scheduling & rendering helpers
list(APPEND PUTTYALT_SOURCES
    src/automation/puttyalt_backoff.c
    src/automation/puttyalt_jobqueue.c
    src/automation/puttyalt_scheduler2.c
    src/automation/puttyalt_statemachine2.c
    src/config/puttyalt_colorparse.c
    src/config/puttyalt_envfile.c
    src/config/puttyalt_unitparse.c
    src/network/puttyalt_bandwidth.c
    src/network/puttyalt_dnscache2.c
    src/network/puttyalt_httprange.c
    src/network/puttyalt_ipv6fmt.c
    src/security/puttyalt_redact.c
    src/security/puttyalt_totpwin.c
    src/session/puttyalt_recentfiles.c
    src/session/puttyalt_sortsess.c
    src/terminal/puttyalt_blinktimer.c
    src/terminal/puttyalt_linewrap2.c
    src/terminal/puttyalt_palette256.c
    src/tools/puttyalt_bitfield.c
    src/tools/puttyalt_crc16.c
    src/tools/puttyalt_fixedpoint.c
    src/tools/puttyalt_interval.c
    src/tools/puttyalt_lrucache.c
    src/tools/puttyalt_minheap.c
    src/tools/puttyalt_movavg.c
    src/tools/puttyalt_quantize.c
    src/tools/puttyalt_statacc.c
    src/tools/puttyalt_varint.c
    src/ui/puttyalt_menutree.c
    src/ui/puttyalt_scrollbar2.c
)

# v3.13.0 protocol parsers, terminal sequences & auth safety
list(APPEND PUTTYALT_SOURCES
    src/automation/puttyalt_cronfield.c
    src/automation/puttyalt_exparser.c
    src/automation/puttyalt_macrorec.c
    src/automation/puttyalt_triggermatch.c
    src/config/puttyalt_jsonptr.c
    src/config/puttyalt_pathnorm.c
    src/config/puttyalt_tomlval.c
    src/network/puttyalt_cookiejar.c
    src/network/puttyalt_httpstatus.c
    src/network/puttyalt_proxyparse.c
    src/network/puttyalt_sshver.c
    src/network/puttyalt_wsframe2.c
    src/security/puttyalt_certsubject.c
    src/security/puttyalt_jwtdecode.c
    src/security/puttyalt_ratelimitauth.c
    src/session/puttyalt_sessfilter.c
    src/terminal/puttyalt_csiparse2.c
    src/terminal/puttyalt_dec_special.c
    src/terminal/puttyalt_mouseev.c
    src/terminal/puttyalt_oscparse.c
    src/tools/puttyalt_ansilen.c
    src/tools/puttyalt_diffhunk.c
    src/tools/puttyalt_hexview.c
    src/tools/puttyalt_kvstore.c
    src/tools/puttyalt_numbase.c
    src/tools/puttyalt_semver.c
    src/ui/puttyalt_dialogfit.c
    src/ui/puttyalt_textfield.c
)

# v3.14.0 rendering, layout, codecs & statistics helpers
list(APPEND PUTTYALT_SOURCES
    src/automation/puttyalt_debounce2.c
    src/automation/puttyalt_pipeline2.c
    src/automation/puttyalt_watchdog2.c
    src/config/puttyalt_mergeconf.c
    src/network/puttyalt_etag.c
    src/network/puttyalt_ftpcode.c
    src/network/puttyalt_retryafter.c
    src/security/puttyalt_permcheck.c
    src/session/puttyalt_sessclone.c
    src/terminal/puttyalt_damage.c
    src/terminal/puttyalt_gridcopy.c
    src/terminal/puttyalt_reflow2.c
    src/terminal/puttyalt_selspan.c
    src/tools/puttyalt_ascii85.c
    src/tools/puttyalt_fuzzyscore.c
    src/tools/puttyalt_jsonesc.c
    src/tools/puttyalt_percentile.c
    src/tools/puttyalt_rndname.c
    src/tools/puttyalt_sparkline2.c
    src/tools/puttyalt_strsplit2.c
    src/tools/puttyalt_timeparse.c
    src/ui/puttyalt_anchorpos.c
    src/ui/puttyalt_badge.c
    src/ui/puttyalt_flexbox.c
    src/ui/puttyalt_layoutgrid.c
    src/ui/puttyalt_tooltip.c
)

# v3.15.0 compression, codecs, bit I/O & protocol parsing
list(APPEND PUTTYALT_SOURCES
    src/automation/puttyalt_conditional.c
    src/automation/puttyalt_throttle2.c
    src/config/puttyalt_defaults.c
    src/config/puttyalt_dotpath.c
    src/network/puttyalt_chunked.c
    src/network/puttyalt_contenttype.c
    src/network/puttyalt_qparams.c
    src/security/puttyalt_saltgen.c
    src/security/puttyalt_tlsver.c
    src/session/puttyalt_sesstats.c
    src/terminal/puttyalt_scrollregion.c
    src/terminal/puttyalt_unicodefold.c
    src/tools/puttyalt_adler32.c
    src/tools/puttyalt_bitreader.c
    src/tools/puttyalt_bitwriter.c
    src/tools/puttyalt_deltaenc.c
    src/tools/puttyalt_ema2.c
    src/tools/puttyalt_hexcodec.c
    src/tools/puttyalt_hufflen.c
    src/tools/puttyalt_lz77.c
    src/tools/puttyalt_romanfmt.c
    src/ui/puttyalt_listview2.c
    src/ui/puttyalt_statusfmt.c
)

# v3.16.0 geometry, graphics math, containers & networking
list(APPEND PUTTYALT_SOURCES
    src/automation/puttyalt_eventcounter.c
    src/automation/puttyalt_retrytable.c
    src/automation/puttyalt_seqrunner.c
    src/config/puttyalt_schemacheck.c
    src/network/puttyalt_keepalivecalc.c
    src/network/puttyalt_subnet.c
    src/network/puttyalt_tcpstate.c
    src/security/puttyalt_fingerprintcmp.c
    src/session/puttyalt_quickconnect.c
    src/terminal/puttyalt_boxdraw.c
    src/terminal/puttyalt_gradient.c
    src/tools/puttyalt_base58.c
    src/tools/puttyalt_circbuf.c
    src/tools/puttyalt_clamp2.c
    src/tools/puttyalt_line2.c
    src/tools/puttyalt_stack2.c
    src/tools/puttyalt_tablefmt.c
    src/tools/puttyalt_vec2.c
    src/ui/puttyalt_colormix.c
    src/ui/puttyalt_easing.c
    src/ui/puttyalt_rectops.c
    src/ui/puttyalt_tabstrip.c
)

# v3.17.0 string algorithms, search, parsing & trie
list(APPEND PUTTYALT_SOURCES
    src/automation/puttyalt_argparse2.c
    src/automation/puttyalt_tokenstream.c
    src/automation/puttyalt_varscope.c
    src/config/puttyalt_inheritchain.c
    src/network/puttyalt_useragent.c
    src/security/puttyalt_passpolicy2.c
    src/session/puttyalt_sessmatch.c
    src/terminal/puttyalt_wrapcursor.c
    src/tools/puttyalt_boyermoore.c
    src/tools/puttyalt_gridmap.c
    src/tools/puttyalt_kmp.c
    src/tools/puttyalt_longestcommon.c
    src/tools/puttyalt_numparse2.c
    src/tools/puttyalt_soundex.c
    src/tools/puttyalt_strmetric.c
    src/tools/puttyalt_strreplace2.c
    src/tools/puttyalt_titlecase.c
    src/tools/puttyalt_trie2.c
    src/tools/puttyalt_wordcount.c
    src/ui/puttyalt_breadcrumb.c
    src/ui/puttyalt_keyhint.c
)

# v3.18.0 HTTP/DNS/SOCKS protocols, terminal attrs & safety
list(APPEND PUTTYALT_SOURCES
    src/automation/puttyalt_guardrail.c
    src/automation/puttyalt_scriptloop.c
    src/automation/puttyalt_statetrack.c
    src/config/puttyalt_profilemerge.c
    src/config/puttyalt_wildcardmatch.c
    src/network/puttyalt_dnsname.c
    src/network/puttyalt_httphdr.c
    src/network/puttyalt_ipclass.c
    src/network/puttyalt_mimetype.c
    src/network/puttyalt_requestline.c
    src/network/puttyalt_socks5.c
    src/security/puttyalt_sessionkey.c
    src/session/puttyalt_conntimer.c
    src/terminal/puttyalt_altbuffer.c
    src/terminal/puttyalt_cellattr.c
    src/terminal/puttyalt_vtparam.c
    src/tools/puttyalt_ipv4sort.c
    src/tools/puttyalt_ratio2.c
    src/ui/puttyalt_contextmenu2.c
    src/ui/puttyalt_notifstack.c
)

# v3.19.0 sessions, config, scheduling, audit & UI state
list(APPEND PUTTYALT_SOURCES
    src/automation/puttyalt_dryrun.c
    src/automation/puttyalt_jobgraph.c
    src/automation/puttyalt_retrybudget.c
    src/automation/puttyalt_schedrule.c
    src/config/puttyalt_backuprotate.c
    src/config/puttyalt_configlint.c
    src/config/puttyalt_themeparse2.c
    src/network/puttyalt_latencystat.c
    src/security/puttyalt_auditline.c
    src/session/puttyalt_bookmarktree.c
    src/session/puttyalt_favorites2.c
    src/session/puttyalt_sessexport.c
    src/terminal/puttyalt_scrollbackidx.c
    src/tools/puttyalt_csvquote.c
    src/tools/puttyalt_listdedup.c
    src/tools/puttyalt_prettybytes.c
    src/tools/puttyalt_timeago2.c
    src/ui/puttyalt_colorpick.c
    src/ui/puttyalt_panelresize.c
    src/ui/puttyalt_searchbar2.c
)

# v3.20.0 numeric algorithms, hashing, RNG & quantization
list(APPEND PUTTYALT_SOURCES
    src/automation/puttyalt_exprcalc.c
    src/config/puttyalt_durationcfg.c
    src/network/puttyalt_httpdate.c
    src/network/puttyalt_ipv6class.c
    src/security/puttyalt_otpwindow.c
    src/session/puttyalt_sessiontag2.c
    src/terminal/puttyalt_colorquant.c
    src/terminal/puttyalt_graphemes.c
    src/tools/puttyalt_fnvhash.c
    src/tools/puttyalt_gcdlcm.c
    src/tools/puttyalt_isqrt.c
    src/tools/puttyalt_mathutil.c
    src/tools/puttyalt_modexp.c
    src/tools/puttyalt_murmur.c
    src/tools/puttyalt_primes.c
    src/tools/puttyalt_xorshift.c
    src/ui/puttyalt_sortindicator.c
    src/ui/puttyalt_virtualscroll.c
)

# v3.21.0 text encoding, Unicode codecs & i18n
list(APPEND PUTTYALT_SOURCES
    src/automation/puttyalt_i18nfmt.c
    src/config/puttyalt_keyboardlayout.c
    src/config/puttyalt_msgcatalog.c
    src/config/puttyalt_plural.c
    src/network/puttyalt_punycode.c
    src/security/puttyalt_unicodesafe.c
    src/session/puttyalt_historysearch.c
    src/terminal/puttyalt_bom.c
    src/terminal/puttyalt_utf16codec.c
    src/terminal/puttyalt_utf8codec.c
    src/tools/puttyalt_caseconv.c
    src/tools/puttyalt_htmlentity.c
    src/tools/puttyalt_latin1conv.c
    src/tools/puttyalt_numlocale.c
    src/ui/puttyalt_charpicker.c
    src/ui/puttyalt_textalign.c
)

# v3.22.0 containers, hash maps, queues & set structures
list(APPEND PUTTYALT_SOURCES
    src/automation/puttyalt_actionqueue.c
    src/automation/puttyalt_fsm2.c
    src/config/puttyalt_layeredconfig.c
    src/network/puttyalt_slidingwindow.c
    src/session/puttyalt_sessionpool.c
    src/terminal/puttyalt_graphemewidth.c
    src/tools/puttyalt_bloomfilter.c
    src/tools/puttyalt_deque2.c
    src/tools/puttyalt_hashmap2.c
    src/tools/puttyalt_linkedlist2.c
    src/tools/puttyalt_objpool.c
    src/tools/puttyalt_priorityq.c
    src/tools/puttyalt_rangeset.c
    src/tools/puttyalt_sortedset.c
    src/tools/puttyalt_unionfind.c
    src/ui/puttyalt_treemodel.c
)

# Wire all feature-module source lists into the library target.
# (Previously these lists were appended but never compiled into puttyalt,
#  so modules like ctxmenu were missing from the GUI link.)
if(PUTTYALT_SOURCES)
  target_sources(puttyalt PRIVATE ${PUTTYALT_SOURCES})
endif()
