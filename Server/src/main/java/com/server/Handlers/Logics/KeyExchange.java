package com.server.Handlers.Logics;

import com.server.Protobufs.KeyMessages;
import com.server.Protobufs.LoginMessages;
import com.server.Utils.Cryptography;
import io.netty.channel.ChannelHandlerContext;

import static com.server.ServerMain.CRYPTOGRAPHY;

public class KeyExchange
{
    public KeyExchange(ChannelHandlerContext ctx, KeyMessages.KeyExchange keyExchange)
    {
        if(keyExchange.getPublicKey().isEmpty()) return;

        if (keyExchange.getState() == LoginMessages.MsgState.MS_INIT)
        {
            System.out.println("Accept public key");

            Cryptography cryptography = ctx.channel().attr(CRYPTOGRAPHY).get();
            String PubKey = cryptography.GenerateKeyPair_DH();
            if (PubKey.isEmpty()) return;

            KeyMessages.KeyExchange msg = KeyMessages.KeyExchange.newBuilder().setPublicKey(PubKey).setState(LoginMessages.MsgState.MS_SUCCESSFUL).build();
            ctx.writeAndFlush(KeyMessages.Wrapper.newBuilder().setKeyexchange(msg).build());
        }

        ctx.channel().attr(CRYPTOGRAPHY).get().ComputeSharedSecretKey_DH(keyExchange.getPublicKey());
    }
}
