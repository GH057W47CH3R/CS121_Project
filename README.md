# CMS

Our project is a basic CMS system that project that supports the following data:

- name
- address
- phone number
- ...

## Layout

The project is laid out with a core library that is used by app. The app
directory is where the executable compiles from. It includes parts of core.
We use cmake to build the project.

## Class Spec

- Record := The data type of the information stored per person.
- RecordArray := is an array of records with an associated size and capacity.
(its meant to mirror std::vector essentially)

## Documentation

To build: (note: the first command only needs to be ran once
future builds only need to call the second.)

```
  cmake -S . -B build
  cmake --build build
```

To run

```
  ./build/app
```
