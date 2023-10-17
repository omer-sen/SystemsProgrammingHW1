Name: Reuben Thomas
netID: rmt135

Name: Omer Sen
netID: 


ERORRS:
To handle errors in both mymalloc() and myfree(), we decide to print out the specifc error to the user,
followed by the file name and line nubmer where the erorr occured.

in mymalloc() we can encouter 2 different errors:

    1. The client requested 0 bytes. In this case we simply print out the error, and return a NULL 
    pointer.

    2. The client requested an amount that is too large to be conatined in te heap. This error occurs
    when all chunks are iterated through, and none are either free or contain enough space to hold the 
    client's data. In this case, the error is reported to the client, along with the number of bytes
    requested, and a NULL pointer is returned.

in myfree() we can encounter 3 different errors:

    1. The client attemtps to free a pointer that was not obtained from the mymalloc(). We can catch
    this by making sure the pointer/address is within the bounds of the static global memory array,
    i.e "memory_start < ptr < memory_end". If not, we print out this error, and return NULL.

    2. Double free

    3. Pointer does not point to the beginning of a chunk. 
    
    **we never look in the contents of pointer unless we're sure it starts at the beginning of chunk

TESTING:

DESIGN NOTES:


TEST PLAN:


DESCRIPTIONS OF TEST PROGRAMS:

DESIGN NOTES:
