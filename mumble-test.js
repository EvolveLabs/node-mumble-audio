var openal = require('./openal');
var mumble = require('electron-mumble');
var speaker = new openal.Device();
var source = new openal.CaptureDevice();
var out_stream = null;

console.log( 'Connecting...' );
mumble.connect( process.env.MUMBLE_SERVER, {}, function ( error, connection ) {
    if( error ) { throw new Error( error ); }

    console.log( 'Connected.' );
    console.log( 'Authenticating...' );

    out_stream = connection.inputStream({sampleRate: 48000});

    connection.authenticate( process.env.MUMBLE_USER, process.env.MUMBLE_PASS );
    connection.on( 'initialized', onInit );
    connection.on( 'voice', onVoice );
});

var onInit = function() {
    console.log( 'Authenticated.' );

    source.on('data', function (data) {
    	out_stream.write(data);
    })
    source.start();

    // Connection is authenticated and usable.
    speaker.play();
};

var onVoice = function( pcmData ) {
    speaker.write(pcmData);
}
