/* Copyright (C) 1991-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses ISO/IEC 10646 (2nd ed., published 2011-03-15) /
   Unicode 6.0.  */
/* We do not support C11 <threads.h>.  */
public class BoundedBufferMain {
    public static void main (String[] args) {
        // Check the arguments of the command line
        if (args.length != 3){
            System.out.println ("PROGRAM N_CONSUMERS N_PRODUCERS BUFFER_SIZE");
            System.exit(1);
        }
        int nConsumers = Integer.parseInt(args[0]);
        int nProducers = Integer.parseInt(args[1]);
        int bufferSize = Integer.parseInt(args[2]);
        // Create a buffer
        BoundedBuffer buffer =
            new SemBoundedBuffer(bufferSize);
        // Create and start consumers
        for (int i = 0; i < nConsumers; i++) new Consumer(i, buffer).start();
        // Create and start producers
        for (int i = 0; i < nProducers; i++) new Producer(i, buffer).start();
        System.out.println("main completed");
    }
}
