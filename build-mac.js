const builder = require('electron-builder');

builder.build({
    config: {
        'appId': 'momosuke.analysis.app',
        'mac':{
            'target': 'zip',
        }
    }
});