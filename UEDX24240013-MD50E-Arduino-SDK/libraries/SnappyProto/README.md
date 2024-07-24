# SnappyProto

This library ports 2 existing libraries internally as there were not Arduino versions of these libraries already

## License Notes

While this repo is licensed Apache2, neither of these libraries are, both are licensed under different and custom licesnes.

Both have permissive licenses, but please read them and make sure your use is compliant with their terms:

* [nanopb](https://github.com/nanopb/nanopb) which is licensed under the [zlib license](https://github.com/grafana/prometheus-arduino/blob/main/src/proto/LICENSE) and code is found in the `src/proto` directory.
* [snappy-c](https://github.com/andikleen/snappy-c) which is licensed under an [custom license](https://github.com/grafana/prometheus-arduino/blob/main/src/snappy/LICENSE) and code is found in the `src/snappy` directory.

Some small modifications were made to both sources to get them to compile and work on Arduino and are tagged with `// E.Welch` 