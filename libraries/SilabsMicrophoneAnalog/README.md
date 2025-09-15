# Silabs Analog Microphone library

This library allows the use of an analog microphone by sampling its data stream continuously. The default configuration and the pin layout in the provided example is compatible with Seeed Studio XIAO MG24 Sense, but the driver can work with any connected analog microphone.

## Methods

## `MicrophoneAnalog(data_pin, enable_pin)`

#### Description

Constructor for the MicrophoneAnalog class. Expects the microphone's data pin and enable pin. The enable pin parameter is optional.

#### Syntax

```
MicrophoneAnalog microphone(D0, D1);
```

## Methods

## `MicrophoneAnalog.begin(buffer, num_samples)`

#### Description

Initializes the ADC to sample microphone data continuously.

#### Syntax

```
MicrophoneAnalog.begin(buffer, num_samples);
```

#### Parameters

buffer: The buffer which will be filled during data sampling.
num_samples: The number of samples taken in one DMA transfer. It has to be the size of the buffer.

## `MicrophoneAnalog.end()`

#### Description

Deinitializes the ADC and ends the microphone sampling.

#### Syntax

```
MicrophoneAnalog.end();
```

#### Parameters

None

## `MicrophoneAnalog.startSampling(callback)`

#### Description

Starts data sampling from the microphone.

#### Syntax

```
MicrophoneAnalog.startSampling(callback)
```

#### Parameters

callback: A user defined callback, which will get called after the specified number of samples have been acquired.

## `MicrophoneAnalog.stopSampling()`

#### Description

Stops sampling data from the microphone.

#### Syntax

```
MicrophoneAnalog.stopSampling();
```

#### Parameters

None

## `MicrophoneAnalog.averageSamples(buffer, buf_size)`

#### Description

Calculates the average of the provided samples.

#### Syntax

```
float average = MicrophoneAnalog.averageSamples(buffer, buf_size);
```

#### Parameters

buffer: The array of elements to be averaged.
buf_size: The size of the buffer.

## `MicrophoneAnalog.getSingleSample()`

#### Description

Acquires a single sample.

#### Syntax

```
uint32_t sample = MicrophoneAnalog.getSingleSample();
```

#### Parameters

None