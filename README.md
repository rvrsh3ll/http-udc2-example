# http-udc2-example

This is an example UDC2 implementation for CrystalC2.

## http-forwarder

An ASP.NET Core application that starts a web server, relays incoming requests to the UDC2 listener and outgoing responses back to the Beacon.

## http-module

A COFF that uses pard0p's [LibWinHttp](https://github.com/pard0p/LibWinHttp) library to communicate with the http-forwarder via HTTP POSTs.