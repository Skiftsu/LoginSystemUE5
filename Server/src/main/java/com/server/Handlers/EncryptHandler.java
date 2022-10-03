package com.server.Handlers;

import com.server.Protobufs.KeyMessages;
import io.netty.channel.ChannelHandlerContext;
import io.netty.handler.codec.MessageToMessageEncoder;


import java.util.List;

import static com.server.ServerMain.CRYPTOGRAPHY;

public class EncryptHandler extends MessageToMessageEncoder<KeyMessages.Wrapper>
{
    @Override
    protected void encode(ChannelHandlerContext channelHandlerContext, KeyMessages.Wrapper wrapper, List<Object> list) throws Exception
    {
        if(!wrapper.hasEncryptedMessage())
        {
            channelHandlerContext.writeAndFlush(wrapper);
            return;
        }

        KeyMessages.EncryptedMessage encryptedMessage = wrapper.getEncryptedMessage();
        Object[] EncryptOut = channelHandlerContext.channel().attr(CRYPTOGRAPHY).get().Encrypt(encryptedMessage.getMessage());
        encryptedMessage = KeyMessages.EncryptedMessage.newBuilder().clear().setMessage((String)EncryptOut[0]).setIv((String)EncryptOut[1]).build();
        channelHandlerContext.writeAndFlush(KeyMessages.Wrapper.newBuilder().setEncryptedMessage(encryptedMessage).build());
    }
}