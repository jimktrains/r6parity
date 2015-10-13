RAID 6 Parity
-------------

RAID 6 uses 2 parity techniques to allow for up to 2 disks to become
unreadable, an xor-based technique, and a Reed-Solomon-based one (which is
based on Galois Fields).  As a way to explore these I built this simple
program based on a PDF from Kernel.org:
https://www.kernel.org/pub/linux/kernel/people/hpa/raid6.pdf

Example output:

    Original: 48 45 4c 4c 4f 
    Parity:   42 31
    
    Missing Datum
    48 45 4c 00 4f 
    48 45 4c 4c 4f 
    Pass: YES
    
    Missing Dataum and RS Parity
    00 45 4c 4c 4f 
    48 45 4c 4c 4f 
    Pass: YES
    
    Missing 2 Data
    48 00 00 4c 4f 
    48 45 4c 4c 4f 
    Pass: YES

