const IOVEC_PTR_OFFSET = 0;
const IOVEC_SIZE_OFFSET = 4;

function bytesToNumber(bytes) {
    let result = 0;
    for (let x of bytes.reverse()) {
        result = result * 0x100 + x;
    }
    return result;
}

function deref(mem, ptr, size) {
    return mem.subarray(ptr, ptr + size);
}

async function start() {
    const utf8decoder = new TextDecoder("utf-8");
    let memoryView = undefined;
    const wasm = await WebAssembly.instantiateStreaming(
        fetch('./main.wasm'),
        {
            "wasi_snapshot_preview1": {
                "proc_exit": (...args) => console.log(`proc_exit(${args})`),
                "fd_write": (fd, iovecs, iovecs_len, nwritten) => {
                    let iovec_ptr = bytesToNumber(deref(memoryView, iovecs + IOVEC_PTR_OFFSET, 4));
                    let iovec_size = bytesToNumber(deref(memoryView, iovecs + IOVEC_SIZE_OFFSET, 4));
                    let message = utf8decoder.decode(deref(memoryView, iovec_ptr, iovec_size));
                    if (fd == 1) {
                        console.log(message);
                    } else if (fd == 2) {
                        console.error(message);
                    } else {
                        console.assert(false, `unknown file descriptor ${fd}`);
                    }
                    
                    {
                        let n = iovec_size;
                        for (let i = 0; i < 4; ++i) {
                            memoryView[nwritten + i] = n % 0x100;
                            n /= 0x100;
                        }
                    }

                    return 0;
                },
                "environ_sizes_get": (...args) => console.log(`environ_sizes_get(${args})`),
                "environ_get": (...args) => console.log(`environ_get(${args})`),
                "fd_prestat_get": (...args) => console.log(`fd_prestat_get(${args})`),
                "fd_prestat_dir_name": (...args) => console.log(`fd_prestat_dir_name(${args})`),
            }
        });
    memoryBuffer = wasm.instance.exports.memory.buffer;
    memoryView = new Uint8Array(wasm.instance.exports.memory.buffer);
    console.log(wasm);
    wasm.instance.exports.main();
}

start().catch((e) => console.error(e));
