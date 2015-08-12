node-mumble-audio
===========

This addon uses openal to provide capture and playback devices compatible with node-mumble. 

## Building:
```
$ npm install electron-build-tools -g
$ elb configure
```

## To Do:
* mac / linux support (need to build openal-soft and drop binaries into deps)
* expose apis for device enumeration and selection.
* Hook up to CI and publish pre-built binaries.

## Limitations:
* Only supports PCM data.
* Assumes a sample rate of 48k.

## Other
See [node-openal](https://github.com/jefftimesten/node-openal) for a starting point to a more general openal api.