kernel
void
ArrayMultAdd( global const float *dA, global const float *dB, global float *dC, global const float *gD )
{
	int gid = get_global_id( 0 );

	dC[gid] = dA[gid] * dB[gid] + gD[gid];
}
