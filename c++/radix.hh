// Copyright(c), Victor J. Duvanenko, 2010
// http://www.drdobbs.com/parallel/parallel-in-place-radix-sort-simplified/229000734?pgno=2
// Function template In-place MSD Radix Sort implementation (simplified).
template< class _Type, unsigned long PowerOfTwoRadix, unsigned long Log2ofPowerOfTwoRadix, long Threshold >
inline void _RadixSort_Unsigned_PowerOf2Radix_L1( _Type* a, long last, _Type bitMask, unsigned long shiftRightAmount )
{
    unsigned long count[ PowerOfTwoRadix ];
    for( unsigned long i = 0; i < PowerOfTwoRadix; i++ )     count[ i ] = 0;
    for ( long _current = 0; _current <= last; _current++ )  count[ (unsigned long)(( a[ _current ] & bitMask ) >> shiftRightAmount ) ]++; // Scan the array and count the number of times each value appears
 
    long startOfBin[ PowerOfTwoRadix + 1 ], endOfBin[ PowerOfTwoRadix ], nextBin = 1;
    startOfBin[ 0 ] = endOfBin[ 0 ] = 0;    startOfBin[ PowerOfTwoRadix ] = -1;         // sentinal
    for( unsigned long i = 1; i < PowerOfTwoRadix; i++ )
        startOfBin[ i ] = endOfBin[ i ] = startOfBin[ i - 1 ] + count[ i - 1 ];
 
    for ( long _current = 0; _current <= last; )
    {
        unsigned long digit;
        _Type _current_element = a[ _current ]; // get the compiler to recognize that a register can be used for the loop instead of a[_current] memory location
        while( endOfBin[ digit = (unsigned long)(( _current_element & bitMask ) >> shiftRightAmount )] != _current )  _swap( _current_element, a[ endOfBin[ digit ]++ ] );
        a[ _current ] = _current_element;
 
        endOfBin[ digit ]++;
        while( endOfBin[ nextBin - 1 ] == startOfBin[ nextBin ] )  nextBin++;   // skip over empty and full bins, when the end of the current bin reaches the start of the next bin
        _current = endOfBin[ nextBin - 1 ];
    }
    bitMask >>= Log2ofPowerOfTwoRadix;
    if ( bitMask != 0 )                     // end recursion when all the bits have been processes
    {
        if ( shiftRightAmount >= Log2ofPowerOfTwoRadix ) shiftRightAmount -= Log2ofPowerOfTwoRadix;
        else                                                shiftRightAmount  = 0;
 
        for( unsigned long i = 0; i < PowerOfTwoRadix; i++ )
        {
            long numberOfElements = endOfBin[ i ] - startOfBin[ i ];
            if ( numberOfElements >= Threshold )     // endOfBin actually points to one beyond the bin
                _RadixSort_Unsigned_PowerOf2Radix_L1< _Type, PowerOfTwoRadix, Log2ofPowerOfTwoRadix, Threshold >( &a[ startOfBin[ i ]], numberOfElements - 1, bitMask, shiftRightAmount );
            else if ( numberOfElements >= 2 )
                insertionSortSimilarToSTLnoSelfAssignment( &a[ startOfBin[ i ]], numberOfElements );
        }
    }
}
 
template< class _Type >
inline void RadixSortInPlace_PowerOf2Radix_Unsigned( _Type* a, unsigned long a_size )
{
    const long Threshold                      =  25;
    const unsigned long PowerOfTwoRadix       = 256;    // Radix - must be a power of 2
    const unsigned long Log2ofPowerOfTwoRadix =   8;    // log( 256 ) = 8
    unsigned long shiftRightAmount = sizeof( _Type ) * 8 - Log2ofPowerOfTwoRadix;       // Create bit-mask and shift right amount
    _Type bitMask = (_Type)( ((_Type)( PowerOfTwoRadix - 1 )) << shiftRightAmount );  // bitMask controls/selects how many and which bits we process at a time
 
    if ( a_size >= Threshold )
        _RadixSort_Unsigned_PowerOf2Radix_L1< _Type, PowerOfTwoRadix, Log2ofPowerOfTwoRadix, Threshold >( a, a_size - 1, bitMask, shiftRightAmount );
    else
        insertionSortSimilarToSTLnoSelfAssignment( a, a_size );
}