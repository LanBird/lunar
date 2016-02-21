# Introduction #

Much of the internal data is represented inside lua as something we call a typed automatic lists. This article describes the lists implementation and how to use them.


# Details #

The list itself provides the type (class) of it's data with a `new` method.
There are two ways to use these lists. The first is to call the `new` method to create new entries. The second is to use the `new` method of the base list to obtain a reference to the object and then insert into the actual list.

```

-- first way:
myitem = myfirstlist:new( "myitemconstructorparameter" )

-- second way:
myitem = lunar.mylisttype:new( "myitemconstructorparameter" )
mysecondlist:add( myitem )

```

The implementation of the actual type and the list happens in C. The list is implemented as a table of a database which allows asynchronous access. The types are responsible to update the table the way the user expects it.