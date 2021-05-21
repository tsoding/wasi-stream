async function start() {
    const wasm = await WebAssembly.instantiateStreaming(
        fetch('./hello.wasm'),
        {
            "js": {
                "print": (x) => console.log(`This function was called by WebAssembly program with argument ${x}`)
            }
        });
    console.log(wasm.instance.exports.hello(34, 35));
    console.log(wasm);
}

start().catch((e) => console.error(e));
