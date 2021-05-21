all: main main.wasm

main: main.rs
	rustc main.rs

main.wasm: main.rs
	rustc --target wasm32-wasi main.rs
