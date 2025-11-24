# CMS

Our project is a basic CMS system that supports the following data:

- name
- address
- phone number
- ...

It is designed with a clear separation between user interface and the data in the file system,
so that we can hopefully use multiple threads later on. We will have different user permissions
for users that can write vs. users that can only read.

Currently we have implemented a cli ui. We load into an array of structs from a .txt file. We have
implemented add and delete functions also but they are not hooked into the ui yet. We save to the
same .txt file at the end.

We plan to implement:

- search
- updates to the file on command usage
- user permissions
- richer records including unique id

## Layout

The project is laid out with a core library that is used by app. The app
directory is where the executable is built from. It includes parts of core.
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
