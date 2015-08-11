To run the demo, first start the IceGrid service:

$ icegridnode --Ice.Config=config.grid

Note that for debug Windows builds you will need to use icegridnoded
rather than icegridnode as the executable name.

In a separate window:

$ icegridadmin --Ice.Config=config.grid -e \
    "application add 'application.xml'"
$ client

This will deploy the application described in the file
"application.xml" and start the client.

If you have already deployed the application, you can update it to try
a new set of descriptors, for example:

$ icegridadmin --Ice.Config=config.grid -e \
    "application update 'application.xml'"

Messages will be displayed in the IceGrid service window.

