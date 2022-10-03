package com.server.Handlers.Logics;

import com.server.DBModels.UserModel;
import com.server.Protobufs.KeyMessages;
import com.server.Protobufs.LoginMessages;
import io.netty.channel.ChannelHandlerContext;
import org.hibernate.Query;
import org.hibernate.Session;

import java.util.List;

import static com.server.ServerMain.*;

public class Login
{
    public Login(ChannelHandlerContext ctx, LoginMessages.Login login) throws Exception
    {
        if(login.getMail().isEmpty() || login.getHash().isEmpty()) return;

        Session session = ctx.channel().attr(HIBER_SESSION).get();

        Query query = session.createSQLQuery("SELECT * FROM UserModel WHERE mail = :mail AND hash = :hash").addEntity(UserModel.class);
        query.setParameter("mail", login.getMail());
        query.setParameter("hash", login.getHash());
        List<UserModel> user = query.list();

        LoginMessages.Login log;
        if(!user.isEmpty()) ctx.channel().attr(CHANNEL_OWNER).set(user.get(0).getLogin());

        log = LoginMessages.Login.newBuilder().setState(user.isEmpty() ? LoginMessages.MsgState.MS_ERROR : LoginMessages.MsgState.MS_SUCCESSFUL).build();
        ctx.writeAndFlush(KeyMessages.Wrapper.newBuilder().setLogin(log).build());
    }
}