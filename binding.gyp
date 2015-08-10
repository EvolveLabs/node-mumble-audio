{
  'targets': [
    {
      'target_name': 'openal-<@(target_arch)',
      'sources': [ 
        "src/openal.cc", 
        "src/WavData.cpp", 
        "src/Device.cpp", 
        "src/Context.cpp", 
        "src/Source.cpp",
        "src/Stream.cpp"
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
                'OpenAL.framework',
              ],
            }
          }
        ],
        ['OS=="win"',
          {
            'defines': [
              '__WINDOWS_MM__'
            ],
            'libraries': [
              "<(module_root_dir)/deps/lib/<@(target_arch)/win/AL/OpenAL32.lib"
            ],
            'msvs_settings': {
                'VCLinkerTool': {
                    'DelayLoadDLLs': [ 'node.dll', 'iojs.exe', 'node.exe' ],
                    'AdditionalOptions': [ '/ignore:4199' ],
                },
            },
          }
        ]
      ]
    },

    # deploy dependencies
    {
      'target_name': 'openal_deploy',
      'type': 'none',
      'dependencies': ['openal-<@(target_arch)'],
      'target_conditions': [
          [ 'OS=="win"', {
            'actions': [
              {
                'action_name': 'soft_oal',
                'inputs': [
                    '<(module_root_dir)/deps/bin/<@(target_arch)/win/AL/soft_oal.dll'
                ],
                'outputs': [
                    '<@(PRODUCT_DIR)/soft_oal.dll'
                ],
                'action': ['cp', '<(module_root_dir)/deps/bin/<@(target_arch)/win/AL/soft_oal.dll', '<@(PRODUCT_DIR)/soft_oal.dll']
              }
            ]
          }
        ]
      ]
    }
  ]
}