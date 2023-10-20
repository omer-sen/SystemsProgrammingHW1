Name: Reuben Thomas
netID: rmt135

Name: Omer Sen
netID: os226

IMPLEMENTATION:
    METADATA FORMAT: 
    We chose to use simple pointer arithmetic to refer to properly iterate through our metadata/heap, without 
    violating the privacy of our clients. Our metadata is 8 bytes: the first 4 refer the size of the entire chunk as an Integer, and 
    the second 4 refer to the state (free or allocated), also as an Integer. Because every pointer will refer to the beginning of the 
    metadata, pointer itself (casted as an int) will represent the size, and "pointer + 4" (casted as an int) will represent the state.
    To get a pointer to the next metadata, we simply obtain the size of the current metadata, and add it to the current pointer. Using 
    these means, we were able to handle the metadata and payloads.

    MACROS & METHODS: 
    We included several different methods that, in summary, allow us to get and set the size and state of the current and next
    metadata. This was done using the aforementioned pointer arithmetic. We also have a getNext() method which returns a pointer to the next 
    metadata, and validPtr() which returns a boolean that conveys whether a given pointer/address is within the range of memory[]. 
    
    For myfree() also need two additional methods. isPrecidingAndFree() is a useful method to be used when iterating through metadatas. It 
    checks whether the current metadata is both preceding the target metadata and is Free itself, and returns a boolean. This can be useful for coalleceing 
    purposes. mergeBlocks() is also a useful method for coalleceing free chunks. It simply updates the size of the preceding metadata,
    thus deleting the latter one, as it is now considered proprietary client data.

    For our macros, we have one called MEMLENGTH that we use to test mymalloc() with different heap sizes. We also have one called ROUNDUP(x) which 
    replaces an integer x with the next highest multiple of 8. This is useful for maintaining alignment in our memory. We also had FREE and 
    ALOC macros to be used when reading and writing chunk states. 

    MYMALLOC:

    MYFREE:

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

    2. Double free, because of the coallesing property of free, if a pointer is freed once, coallesed
		with another chunk, and freed again, there is no way of know if it used to point to a real chunk,
		because that data is now owned by another client. We could create a struct that kept track of all
		pointers, but we decided this was too much overhead only to report a specific kind of error.
		Instead we report a double free error twice.

    3. Pointer does not point to the beginning of a chunk. 
    
    **we never look in the contents of pointer unless we're sure it starts at the beginning of chunk

TESTING:
    TEST PLAN:


    DESCRIPTIONS OF TEST PROGRAMS:

