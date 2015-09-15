{
  'targets': [
    {
      'target_name': 'node-mumble-audio-<@(target_arch)',
      'win_delay_load_hook': 'false',
      'sources': [ 
        "src/init.cc",
        "src/ALCaptureDevice.cpp", 
        "src/ALCaptureWorker.cpp",
        "src/ALPlaybackDevice.cpp",
        "src/ALPlaybackWorker.cpp"
      ],
      'include_dirs': [
        "deps/includes",
        "<!(node -e \"require('nan')\")",
        "<!(node -e \"require('electron-updater-tools')\")"
      ],
      'conditions': [
        ['OS=="linux"',
          {
            'cflags_cc!': [
              '-fno-exceptions'
            ],
            'defines': [
              '__LINUX_ALSASEQ__'
            ],
            'link_settings': {
              'libraries': [
                '-lal'
              ]
            }
          }
        ],
        ['OS=="mac"',
          {
            'defines': [
              '__MACOSX_CORE__'
            ],
            'xcode_settings': {
              'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
            },
            'link_settings': {
              'libraries': [
                '<(module_root_dir)/deps/bin/<(target_arch)/mac/AL/libopenal.1.16.0.dylib',
              ]
            }
          }
        ],
        ['OS=="win"',
          {
            'defines': [
              '__WINDOWS_MM__'
            ],
            'libraries': [
              "-lShlwapi.lib",
              "<(module_root_dir)/deps/lib/<@(target_arch)/win/AL/OpenAL32.lib"
            ],
            'msvs_settings': {
              'VCLinkerTool': {
                'DelayLoadDLLs': [ 'node.dll', 'iojs.exe', 'node.exe' ],
                'AdditionalOptions': [ '/ignore:4199' ],
              }
            }
          }
        ]
      ]
    },

    # deploy dependencies
    {
      'target_name': 'openal_deploy',
      'type': 'none',
      'dependencies': ['node-mumble-audio-<@(target_arch)'],
      'target_conditions': [
        [ 'OS=="win"', {
          'actions': [
            {
              'action_name': 'soft_oal',
              'inputs': [
                  '<(module_root_dir)/deps/bin/<@(target_arch)/win/AL/soft_oal.dll'
              ],
              'outputs': [
                  '<@(PRODUCT_DIR)/openal32.dll'
              ],
              'action': ['cp', '<(module_root_dir)/deps/bin/<@(target_arch)/win/AL/soft_oal.dll', '<@(PRODUCT_DIR)/openal32.dll']
            }
          ]
        }],
        [ 'OS=="mac"', {
          'actions': [
            {
              'action_name': 'soft_oal',
              'inputs': [
                '<(module_root_dir)/deps/bin/<@(target_arch)/mac/AL/libopenal.1.16.0.dylib'
              ],
              'outputs': [
                '<@(PRODUCT_DIR)/libopenal.1.16.0.dylib'
              ],
              'action': ['cp', '<(module_root_dir)/deps/bin/<@(target_arch)/mac/AL/libopenal.1.16.0.dylib', '<@(PRODUCT_DIR)/libopenal.1.16.0.dylib']
            }
          ]
        }]
      ]
    }
  ]
}