#pragma once

template< typename T >
void DirectXDeallocator( T * resource )
{
	if( resource ) resource->Release();
}