# Silabs Arduino TensorFlow Lite Micro Library

This library enables the creation of applications that use TensorFlow Lite for Microcontrollers on Arduino Silabs boards. TensorFlow Lite for Microcontrollers is a framework that provides tools for running neural network inference on microcontrollers.

## Hardware-accelerated Kernel
Silicon Labs integrates TensorFlow Lite for Microcontrollers with the Gecko SDK, providing the full framework and automatically selecting the most optimal kernel implementation for the target board.

Select Silicon Labs microcontrollers utilize MVP hardware-accelerated kernels. The MVP accelerator is a co-processor designed for matrix and vector operations, enhancing inference performance.

## Compatibility
This library is currently compatible with the following boards:

 - Arduino Nano Matter
 - Seeed Studio XIAO MG24
 - Silicon Labs xG24 Explorer Kit
 - Silicon Labs xG24 Dev Kit

## Importing a TensorFlow Lite Model into the Library

This guide explains how to create, convert, and integrate a TensorFlow Lite (TFLite) model for use with the Silabs Arduino TensorFlow Lite Micro library.

### Creating the TFLite Model
First, create and train your model using [TensorFlow](https://www.tensorflow.org/). Then, use the following code to convert your TensorFlow model into a TensorFlow Lite model:

```python
converter = tf.lite.TFLiteConverter.from_keras_model(model)
converter.optimizations = [tf.lite.Optimize.DEFAULT]
converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]
converter.inference_input_type = tf.float32 # This depends on your input model type
converter.inference_output_type = tf.float32 # This depends on your output model type

tflite_model = converter.convert()
```

### Converting the Model into Source Files

1. Install the Silicon Labs Configurator (SLC-CLI) Command Line Interface tool. You can find it [here](https://www.silabs.com/developer-tools/simplicity-studio/configurator-command-line-interface).

2. Run the following command to generate the necessary files. Use absolute paths for both the input and output directories:

    ```bash
    slc tflite generate -contentFolder /absolute/path/to/tflite/folder -generationOutput /absolute/path/to/output/folder
    ```

The generated files will be located in the specified output directory and include:

- `sl_tflite_micro_model.c`
- `sl_tflite_micro_model.h`
- `sl_tflite_micro_opcode_resolver.h`

### Copying the Generated Files into the Library
1. Copy `sl_tflite_micro_model.c` to your sketch folder.
2. Ignore `sl_tflite_micro_model.h`.
3. Ignore `sl_tflite_micro_opcode_resolver.h`.

## Usage

To use the library in your project, follow these steps:

1. Include the library
    ```c++
    #include "SilabsTFLiteMicro.h"
    ```

2. Initialize the Silabs TFLite Micro Library
    ```c++
    sl_tflite_micro_init();
    ```

3. Obtain pointers to the model's tensors
    ```c++
    static TfLiteTensor* model_input;
    static tflite::MicroInterpreter* interpreter;
    static TfLiteTensor* model_output;

    model_input = sl_tflite_micro_get_input_tensor();
    interpreter = sl_tflite_micro_get_interpreter();
    model_output = sl_tflite_micro_get_output_tensor();
    ```

4. Feed input data to the model

    Assign input data to the `model_input->data` variable.

5. Run inference
    ```c++
    TfLiteStatus invoke_status = interpreter->Invoke();
    ```

6. Read the result

    Access the output data through the `model_output->data` variable.

## Examples

- `magic_wand`: Gesture recognition using an IMU sensor, leveraging hardware acceleration with the Silabs TFLite Micro library.
- `magic_wand_imu_capture`: Captures and logs motion data using the IMU sensor in CSV format.
