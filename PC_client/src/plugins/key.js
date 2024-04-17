function createCommonToken(params) {
    const access_key = base64ToArrayBuffer(params.author_key);

    const version = params.version;
    let res = 'userid' + '/' + params.user_id;
    const et = Math.ceil((Date.now() + 365 * 24 * 3600 * 1000) / 1000);
    const method = 'sha1';

    const key = `${et}\n${method}\n${res}\n${version}`;

    return new Promise((resolve, reject) => {
        const data = new TextEncoder().encode(key);
        window.crypto.subtle.importKey(
            'raw',
            access_key,
            { name: 'HMAC', hash: { name: 'SHA-1' } },
            false,
            ['sign']
        ).then(key => {
            window.crypto.subtle.sign('HMAC', key, data).then(signature => {
                const sign = arrayBufferToBase64(signature);
                res = encodeURIComponent(res);
                const token = `version=${version}&res=${res}&et=${et}&method=${method}&sign=${sign}`;
                resolve(token);
            }).catch(error => reject(error));
        }).catch(error => reject(error));
    });
}

function base64ToArrayBuffer(base64) {
    const binaryString = window.atob(base64);
    const bytes = new Uint8Array(binaryString.length);
    for (let i = 0; i < binaryString.length; i++) {
        bytes[i] = binaryString.charCodeAt(i);
    }
    return bytes;
}

function arrayBufferToBase64(arrayBuffer) {
    let binary = '';
    const bytes = new Uint8Array(arrayBuffer);
    const len = bytes.byteLength;
    for (let i = 0; i < len; i++) {
        binary += String.fromCharCode(bytes[i]);
    }
    return window.btoa(binary);
}

export { createCommonToken };
