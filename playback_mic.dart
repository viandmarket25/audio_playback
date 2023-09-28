// Copyright (c) 2019, the Dart project authors.  Please see the AUTHORS file
// for details. All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.

import 'dart:ffi' as ffi;
import 'dart:io' show Platform, Directory;
import 'package:path/path.dart' as path;

// FFI signature of the hello_world C function
//typedef HelloWorldFunc = ffi.Void Function();

typedef FfiFunction =ffi.Void Function();
typedef ffiTest = void Function();

void main() {
  // Open the dynamic library
  var libraryPath =
      path.join(Directory.current.path, 'audio_lib', 'libaudio_lib.so');

  if (Platform.isMacOS) {
    libraryPath =
        path.join(Directory.current.path, 'audio_lib', 'libaudio_lib.dylib');
  }

  if (Platform.isWindows) {
    libraryPath = path.join(
        Directory.current.path, 'audio_lib', 'Debug', 'audio_lib.dll');
  }


  // ::::::::: dynamic library containing c function
  final dylib = ffi.DynamicLibrary.open(libraryPath);


  final  ffiTest ffit = dylib
      .lookup<ffi.NativeFunction<FfiFunction>>('testFFi')
      .asFunction();


 ffit();


  // Call the function
  //hello();
}