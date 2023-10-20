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

    METHODS & MACROS: 
    We included several different methods that, in summary, allow us to get and set the size and state of the current and next
    metadata. This was done using the aforementioned pointer arithmetic. We also have a getNext() method which returns a pointer to the next 
    metadata, and validPtr() which returns a boolean that conveys whether a given pointer/address is within the range of memory[]. 
    
    For myfree() we also needed two additional methods. isPrecidingAndFree() is a useful method to be used when iterating through metadatas. It 
    checks whether the current metadata is both preceding the target metadata and is Free itself, and returns a boolean. This can be useful for coalescing 
    purposes. mergeBlocks() is also a useful method for coalescing free chunks. It simply updates the size of the preceding metadata,
    thus deleting the latter one, as it is now considered proprietary client data.

    For our macros, we have one called MEMLENGTH that we use to test mymalloc() with different heap sizes. We also have one called ROUNDUP(x) which 
    replaces an integer x with the next highest multiple of 8. This is useful for maintaining alignment in our memory. We also had FREE and 
    ALOC macros to be used when reading and writing chunk states. 

    MYMALLOC:
    Our procedure for mymalloc is as follows:
        1. roundup the size to next highest multiple of 8
        2. iterate through each metadata checking if it's chunksize==0 OR big enough to hold the client's data, it's metadata, and the next metadata
            2a. If either are true we first set the size of this metadata to "size+8" (+8 for the metadata) and the state to free (i.e. 1). Now we need to 
            set up the size and state next metadata
                2aa. if the chunk size was 0, it means that the memory is uninitialized. Therefore, the size of the next metadata is simply "MEMLENGTH - (size_of_current_chunk)"
                2ab. else, the next metadata has size "chunk_size - (size+8)"
                2ac. the state of the next metadata will always be free (i.e. 1)
                2ad. now return "ptr+8". The "+ 8" ensures we return a pointer to the beginning of the payload.
        3. if the loop terminates it means that there is no space in the heap for the requested amount of BYTES, so we report this error and return NULL.

    MYFREE:
    Our procedure for myfree is as follows:
        1. check if the pointer is valid using validPtr()
        2. if so, subtract 8 from ptr and refer to it as real ptr
            ** ptr refers to the start of a chunk, so ptr-8 will refer to that chunk's metadata
            ** whether ptr actually points to a beginning of a chunk or not doesn't risk the chance that we accidentally access proprietary data 
            because we will only compare the ptr (address) with our iterator. We only access realPtr when we're certain it's a real pointer.
        3. iterate through each metadata checking if:
            3a. isPrecedingAndFree(curr, realPtr) == true. if so merge curr and realPtr, and if the next chunk is free, merge that as well.
            3b. else, if (realPtr == curr), it means we've found the ptr, and no free chunk precedes it. Therefore we simply check if the next chunk is free, if so we merge.
            Then we set the state of realPtr to free.
        4. if the loop terminates, it means that the ptr does to point to a valid metadata, in which case we return NULL

ERRORS:
To handle errors in both mymalloc() and myfree(), we decide to print out the specific error to the user,
followed by the filename and line number where the error occurred.

In mymalloc() we can encounter 2 different errors:

    1. The client requested 0 bytes. In this case we simply print out the error, and return a NULL 
    pointer.

    2. The client requested an amount that is too large to be contained in the heap. This error occurs
    when all chunks are iterated through, and none are either free or contain enough space to hold the 
    client's data. In this case, the error is reported to the client, along with the number of bytes
    requested, and a NULL pointer is returned.

In myfree() we can encounter 3 different errors:

    1. The client attempts to free a pointer that was not obtained from the mymalloc(). We can catch
    this by making sure the pointer/address is within the bounds of the static global memory array,
    i.e "memory_start < ptr < memory_end". If not, we print out this error, and return NULL.

    2. Pointer does not point to the beginning of a payload. This occurs when the metadata-iterating loop terminates. 
    Because we've already checked if the pointer was within the bounds of the heap, the only possible way this happens is 
    if the client did not pass a correct pointer to the beginning of a payload.  In this case we print out this error,
    and return NULL

    2. Double free. because of the coalescing property of free, if a pointer is freed once, coalesced
    with another chunk, and freed again, there is no way of knowing if it used to point to a real chunk,
    because that data is now owned by another client. We could create a struct that kept track of all
    pointers, but we decided this was too much overhead only to report a specific kind of error.
    Instead we report a double free error twice. Once if a freed metadata is attempted to be freed twice, and another time when 
    the metadata-iterator terminates, which means the given pointer does not point to the start of a chunk. Again this could be 
    because the client attempted to free a chunk that has already been freed AND coalesced. In this case we print out this error,
    and return NULL.


TESTING:
    TEST PLAN:
    
    1. The client attempts to allocate 1 byte memory 120 times, and immediately frees the allocated memory. This test is run 50 times

    2. The client uses malloc() to get 120 1-byte objects, storing the pointers in an array, then uses free() to deallocate the chunks.

    3. The client creates an array of 120 pointers, and randomly allocates or frees the memory in the array. This is continued until the client
    allocated memory 120 times. The rest of the memory is freed afterwards by the client.

    4. The client uses malloc() to get 120 1-byte objects, storing the pointers in an array, then uses free() to deallocate the chunks.
    Afterwards, the client tries to allocate a huge chunk, specifically, MEMLENGTH - 16. Number 16 represents the metadata that is held in the memory.

    5. The client uses randomization to either allocate the whole memory, MEMLENGTH - 16 and free it immediately, or allocate 1 byte memory and free it immediately.

    DESCRIPTIONS OF TEST PROGRAMS:



