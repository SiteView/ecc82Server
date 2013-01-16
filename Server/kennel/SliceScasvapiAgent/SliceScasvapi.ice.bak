// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#ifndef SLICESCASVAPI_ICE
#define SLICESCASVAPI_ICE

module SliceScasvapi
{

dictionary<string , string> SNodeData;
sequence<SNodeData> SForestList;
dictionary<string , SNodeData> SForestMap;

interface ScasvapiMethod
{

//从 svdb 服务器请求获得的数据       传入的请求     返回的错误信息 便于调试
idempotent bool SliceGetUnivData(SNodeData inwhat, out SForestMap fmap,  out string estr);

//提交给 svdb 服务器的数据/及返回数据     传入的请求       返回的错误信息 便于调试
idempotent bool SliceSubmitUnivData(SNodeData inwhat, SForestMap ifmap, out SForestMap ofmap,  out string estr);

//从 svdb 服务器请求获得的树数据      传入的请求     返回的错误信息 便于调试
 idempotent bool SliceGetForestData(SNodeData inwhat, out SForestList flist,  out string estr);

idempotent void shutdown();
};
};

#endif
