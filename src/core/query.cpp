// SELECT * -- outputs all records
// SELECT <name> -- outputs records that match that name
// so to be used by select we need to turn these things into predicates
// true
// name == <name>
// name == <name> && address == <address>

// so we need to parse the strings into a boolean tree or something.
// for now we can just allow simple stuff.
// this file will house the implementation of select, select_mut, edit
//
