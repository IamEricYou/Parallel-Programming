kernel
void
AutoCorrelate( global const float *dArray, global float *dSums )
{
	int Size = get_global_size( 0 );	// the dArray size is actually twice this big
    int gid  = get_global_id( 0 );
	int shift = gid;

	float sum = 0.;
	for( int i = 0; i < Size; i++ )
	{
		sum += dArray[i] * dArray[i + shift];
	}
	dSums[shift] = sum;
}