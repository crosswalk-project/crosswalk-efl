{
  'targets': [
  {
    'target_name': 'xwalk_tizen_lib',
    'type': 'static_library',
    'dependencies': [
      '../build/system.gyp:tizen_sensor',
      '../build/system.gyp:tizen_vibration',
    ],
    'include_dirs': [
      '../..',
    ],
    'sources': [
      'mobile/sensor/sensor_provider.cc',
      'mobile/sensor/sensor_provider.h',
      'mobile/sensor/tizen_data_fetcher_shared_memory.cc',
      'mobile/sensor/tizen_data_fetcher_shared_memory.h',
      'mobile/sensor/tizen_platform_sensor.cc',
      'mobile/sensor/tizen_platform_sensor.h',

      # Broken with Chromium M41: XWALK-3398.
      # 'browser/vibration/vibration_provider_tizen.cc',
      # 'browser/vibration/vibration_provider_tizen.h',
    ],
    'conditions': [
      ['xwalk_link_against_chromium_ewk==0', {
        'dependencies': [
          '../../skia/skia.gyp:skia',
        ],
      }],
      [ 'tizen_mobile == 1', {
        'conditions': [
          ['xwalk_link_against_chromium_ewk==0', {
            'dependencies': [
              '../../ui/base/ui_base.gyp:ui_base',
            ],
          } , {
            'include_dirs': [
              '..',
              '<(DEPTH)/third_party/skia/include/config/',
              '<(DEPTH)/third_party/skia/include/core/',
            ],
          }],
        ],
        'sources': [
          'mobile/ui/tizen_plug_message_writer.cc',
          'mobile/ui/tizen_plug_message_writer.h',
          'mobile/ui/tizen_system_indicator.cc',
          'mobile/ui/tizen_system_indicator.h',
          'mobile/ui/tizen_system_indicator_watcher.cc',
          'mobile/ui/tizen_system_indicator_watcher.h',
          'mobile/ui/tizen_system_indicator_widget.cc',
          'mobile/ui/tizen_system_indicator_widget.h',
          'mobile/ui/widget_container_view.cc',
          'mobile/ui/widget_container_view.h',
        ],
      }],
    ],
  }],
}
