Example code: SubscribeGyro module
===========================

This is an example of a module subscribing to an event.
It implements a callback function that makes NAO speak every time its right
subscribegyro is pressed.

The whole example is available here: :download:`subscribegyro.zip <../subscribegyro.zip>`

Here are the sources of the module:

Header:
-------

:download:`subscribegyro.h <subscribegyro.h>`

.. literalinclude:: subscribegyro.h
   :language: cpp

Source:
-------

:download:`subscribegyro.cpp <subscribegyro.cpp>`

.. literalinclude:: subscribegyro.cpp
   :language: cpp

Main:
-----

:download:`main.cpp <main.cpp>`

.. literalinclude:: main.cpp
   :language: cpp

CMakeLists.txt:
---------------

The corresponding CMakeLists.txt file is:

:download:`CMakeLists.txt <CMakeLists.txt>`

.. literalinclude:: CMakeLists.txt
   :language: cmake

