package com.server.Handlers;

import com.server.Handlers.Logics.KeyExchange;
import io.netty.channel.ChannelHandlerContext;
import io.netty.handler.codec.MessageToMessageDecoder;
import io.netty.handler.timeout.IdleState;
import io.netty.handler.timeout.IdleStateEvent;
import org.hibernate.Session;
import org.hibernate.Transaction;

import java.util.List;

import com.server.Protobufs.*;
import com.server.Utils.Cryptography;
import com.server.Utils.SessionUtil;
import com.server.Handlers.Logics.CheckRegParameter;
import com.server.Handlers.Logics.Login;
import com.server.Handlers.Logics.Registration;

import static com.server.ServerMain.*;

public class DecryptHandler extends MessageToMessageDecoder<KeyMessages.Wrapper>
{
    private final Session session = new SessionUtil().getSession();
    private final Transaction transaction  = session.beginTransaction();
    private final Cryptography cryptography = new Cryptography();
    private boolean bInit = false;

    /* Сохраните необходимые значения во всех обработчиках этого канала */
    private void init(ChannelHandlerContext ctx)
    {
        if(bInit) return;

        bInit = true;
        ctx.channel().attr(HIBER_SESSION).set(session);
        ctx.channel().attr(HIBER_TRANSACTION).set(transaction);
        ctx.channel().attr(CRYPTOGRAPHY).set(cryptography);
    }

    @Override
    protected void decode(ChannelHandlerContext channelHandlerContext, KeyMessages.Wrapper wrapper, List<Object> list) throws Exception
    {
        init(channelHandlerContext);

        if(wrapper.hasEncryptedMessage())
        {
            byte[] original = cryptography.Decrypt(wrapper.getEncryptedMessage().getMessage(), wrapper.getEncryptedMessage().getIv());
            switch (wrapper.getEncryptedMessage().getMessageType())
            {
                case MT_LOGIN:
                    wrapper = KeyMessages.Wrapper.newBuilder().clear().setLogin(LoginMessages.Login.parseFrom(original)).build();
                    break;
                case MT_REGISTRATION:
                    wrapper = KeyMessages.Wrapper.newBuilder().clear().setRegistration(LoginMessages.Registration.parseFrom(original)).build();
                    break;
                default: return;
            }
        }

        if (wrapper.hasCheckRegParameterStatus()) new CheckRegParameter(channelHandlerContext, wrapper);
        else if (wrapper.hasRegistration()) new Registration(channelHandlerContext, wrapper.getRegistration());
        else if (wrapper.hasLogin()) new Login(channelHandlerContext, wrapper.getLogin());
        else if (wrapper.hasKeyexchange()) new KeyExchange(channelHandlerContext, wrapper.getKeyexchange());
    }

    private void clearPlayerData(ChannelHandlerContext ctx)
    {
        if(session.isOpen()) session.close();
        ctx.close();
    }

    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception
    {
        cause.printStackTrace();
        clearPlayerData(ctx);
    }

    @Override
    public void channelUnregistered(ChannelHandlerContext ctx) throws Exception
    {
        super.channelUnregistered(ctx);
        System.out.println("Close connection id: " + ctx.channel().id().asShortText() + " cause ChannelUnregistered");
        clearPlayerData(ctx);
    }

    @Override
    public void userEventTriggered(ChannelHandlerContext ctx, Object evt) throws Exception
    {
        if ((evt instanceof IdleStateEvent stateEvent) && (stateEvent.state() == IdleState.READER_IDLE))
        {
            System.out.println("Close connection id: " + ctx.channel().id().asShortText() + " cause READER_IDLE");
            clearPlayerData(ctx);
        }
    }
}