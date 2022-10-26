# enumerator
Is a library/executable that create a file from a template for every variation. This library/executable allows generating an enumerations of inputs. Enumeration interface allows extension to different types of values/lists.

The minimum required options is a path to an input file:

```bash
    ./enumerator --input=path/to/input.yaml
```

## Template String Example
In this example a string is provided in the template file directly.  Arguments in the template are replaced with specified enumerations if they are in the template as {{varName}}.  The built in argument {{id}} is automatically supplied and is replaced with the specific case id. 

### templateStringExample.yaml
```yaml
---
name: exampleFileGen.
enumerations:
  - !enumerator::enumerations::List
    name: mu
    values: [ .1, .2, .3 ]
  - !enumerator::enumerations::List
    name: density
    values: [ 1.1, 1.2 ]
  - !enumerator::enumerations::Range
    name: vel
    start: .25
    end: .5
    points: 3
generator:
  !enumerator::generators::TemplateGenerator
  template: |
    #!/bin/bash
    echo "Hello World {{mu}} and {{density}} with vel {{vel}} from case {{id}}"
  extension: .sh
  outputDirectory: outputs
reporters:
  - !enumerator::reporters::StreamReporter
```


## Template File Example
In this example uses a template file for the basis.

### templateFileExample.yaml
```yaml
---
name: exampleFileGen.
enumerations:
  - !enumerator::enumerations::List
    name: mu
    values: [ .1, .2, .3 ]
  - !enumerator::enumerations::List
    name: density
    values: [ 1.1, 1.2 ]
generator:
  !enumerator::generators::FileTemplateGenerator
  path: template.sh
reporters:
  - !enumerator::reporters::StreamReporter
```

### template.sh
```shell
#!/bin/bash

echo "Hello World {{mu}} and {{density}} from case {{id}}"
```

## Building Enumerator
The enumerator project can be built using CMake.  Examples and additional details are provided by [CMake](https://cmake.org/runningcmake/).  

## Running Tests Locally
The tests can be run locally using an IDE or cmake directly (ctest command).  To run the tests using the testing environment (docker), first make sure that [Docker](https://www.docker.com) installed.

```bash
# Build the docker testing image
docker build -t testing_image -f DockerTestFile .

# Run the built tests and view results
docker run --rm testing_image 

```