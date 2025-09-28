# EclipseDumper

A modern, configurable MHY0 IL2CPP dumper with clean architecture and versioned offset support.

## Overview

EclipseDumper extracts C#-style type and method definitions from IL2CPP titles. It is built to be:

- Extensible for different games/engines.
- Maintainable with clear separation of concerns and proper abstraction layers.
- Configurable via per-title JSON (no rebuild required to change offsets/behavior).

> Educational and research purposes only. You are responsible for how you use this software.

## Features

- C# definition output (classes, fields, methods).
- Optional `script.json` for IDA-friendly naming.
- Per-title JSON configuration: offsets, detection, behavior flags.
- Full or short type names switchable at runtime (`useFullNames`).
- Static-linked IL2CPP support in main executable (`useMainModule`).

## Architecture

- `Source/Core` — configuration, factories, and engine-agnostic interfaces.
- `Source/Engines/Il2Cpp` — IL2CPP-specific implementation (API wrappers and renderers).
- Clean layering: `Configuration` → `EngineFactory` → `IDumperEngine` → renderer.
- Modern C++ practices, consistent naming, minimal but clear comments.

## Build

1. Open the solution in Visual Studio (x64, Debug/Release).
2. Build the `EclipseDumper` project.

## Usage

1. Prepare a JSON config for your target under the app directory (e.g., `HNA.json`).
2. Launch the target game. Inject the built DLL using your preferred injector.
3. Wait until the game reaches a stable point (e.g., login screen), then trigger the dump (follow your injector’s hotkey/console prompt).

Notes:
- Do not trigger too early; IL2CPP may not be initialized yet.
- If the game statically links IL2CPP (no `GameAssembly.dll`), set `useMainModule: true`.

## Configuration

EclipseDumper loads a per-title JSON file describing engine offsets and behavior. Example:

```json
{
  "gameName": "HNA",
  "version": "0.3.0",
  "engineId": "il2cpp",
  "moduleNames": ["GameAssembly.dll"],
  "processNames": ["NexusAnima.exe"],
  "outputFilePrefix": "HNA",
  "generateScriptJson": false,
  "maxTypeProbeCount": 2000000,
  "useFullNames": false,
  "useMainModule": true,

  "GetTypeInfoFromTypeDefinitionIndex": "0x1000000",

  "ClassGetName": "0x1010000",
  "ClassGetNamespace": "0x1010010",
  "ClassGetMethods": "0x1010020",
  "ClassGetFields": "0x1010030",
  "ClassGetFlags": "0x1010040",
  "ClassGetType": "0x1010050",
  "ClassGetParent": "0x1010060",
  "ClassIsEnum": "0x1010070",
  "ClassGetInterfaces": "0x1010080",
  "ClassFromName": "0x1010090",
  "ClassGetMethodFromName": "0x0",
  "ClassInit": "0x1020000",
  "FromIl2CppType": "0x1030000",

  "MethodGetName": "0x1100000",
  "MethodGetReturnType": "0x1100010",
  "MethodGetParamCount": "0x1100020",
  "MethodGetParam": "0x1100030",
  "MethodGetParamName": "0x1100040",
  "MethodPointer": "0",
  "MethodGetFlags": "54",

  "FieldGetName": "0x1200000",
  "FieldGetType": "0x1200010",
  "FieldGetFlags": "0x1200020",
  "FieldGetOffset": "0x1200030",
  "FieldStaticGetValue": "0x1200040",

  "TypeGetName": "0x1300000",
  "ClassIsValueType": "0x1300010",

  "RuntimeClassInit": "0x1400000",
  "VmClassInit": "0x1400010",
  "ThreadAttach": "0x1400020",
  "VmArrayNew": "0x1400030",
  "ObjectNew": "0x1400040",
  "RuntimeInvoke": "0x1400050",

  "DomainGet": "0x1500000",
  "DomainGetAssemblies": "0x1500010",
  "DomainAssemblyOpen": "0x1500020",
  "AssemblyGetImage": "0x1500030",

  "GcDisable": "0x1600000"
}
```

Guidance:

- `useFullNames`: print fully-qualified names via `TypeGetName`; otherwise use short class names.
- `useMainModule`: if true, RVAs are computed from the main EXE (`GetModuleHandleA(nullptr)`) for static-linked titles. Otherwise, `GameAssembly.dll` is used.
- `MethodPointer` and `MethodGetFlags` are byte offsets within `Il2CppMethod` (not addresses).
- `MethodGetFlags` accepts hex ("0x..") or quoted decimal (e.g. "54").
-  btw, you can add more apis it isn't limited in anyway

## Contributing

Pull requests are welcome. Keep changes aligned with the architecture.

## References

- [Rumi's Blog](https://blog.rumi.dev/blog/genshin-runtime-dump)
- [GracefulDumper](https://github.com/thexeondev/GracefulDumper)
- [djKaty – IL2CPP internals (series)](https://katyscode.wordpress.com/2020/06/24/il2cpp-part-1/)
- [libil2cpp](https://github.com/MlgmXyysd/libil2cpp)
- [IL2CPPRuntimeDumper](https://github.com/lilmayofuksu/Il2CppRuntimeDumper)


## Specialized Wiki
- soon™

## License

MIT. See `LICENSE`.
