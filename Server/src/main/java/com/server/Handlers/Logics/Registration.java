package com.server.Handlers.Logics;

import com.server.DBModels.UserModel;
import com.server.Protobufs.KeyMessages;
import com.server.Protobufs.LoginMessages;
import io.netty.channel.ChannelHandlerContext;
import org.hibernate.Query;
import org.hibernate.Session;
import org.hibernate.Transaction;


import static com.server.ServerMain.*;

public class Registration
{
    public Registration(ChannelHandlerContext ctx, LoginMessages.Registration registration)
    {
        if (registration.getState() == LoginMessages.MsgState.MS_INIT)
        {
            if (CheckUser(ctx, registration)) return;

            SaveUser(ctx, registration);
            registration = LoginMessages.Registration.newBuilder().clear().setState(LoginMessages.MsgState.MS_SUCCESSFUL).build();
        }
        else registration = LoginMessages.Registration.newBuilder().clear().setState(LoginMessages.MsgState.MS_ERROR).build();

        ctx.writeAndFlush(KeyMessages.Wrapper.newBuilder().setRegistration(registration).build());
    }

    private boolean CheckUser(ChannelHandlerContext ctx, LoginMessages.Registration registration)
    {
        if(registration.getLogin().isEmpty() || registration.getMail().isEmpty()) return false;

        Session session = ctx.channel().attr(HIBER_SESSION).get();

        Query query = session.createQuery("SELECT login, mail FROM UserModel WHERE login = :login OR mail = :mail ");
        query.setParameter("login", registration.getLogin());
        query.setParameter("mail", registration.getMail());

        java.util.List<String[]> userdata = query.list();

        return !userdata.isEmpty();
    }

    public void SaveUser(ChannelHandlerContext ctx, LoginMessages.Registration registration)
    {
        UserModel user = new UserModel();
        user.setLogin(registration.getLogin());
        user.setHashPass(registration.getHash());
        user.setMail(registration.getMail());

        Session session = ctx.channel().attr(HIBER_SESSION).get();
        Transaction transaction = ctx.channel().attr(HIBER_TRANSACTION).get();

        session.save(user);
        transaction.commit();

        ctx.channel().attr(CHANNEL_OWNER).set(user.getLogin());
    }
}
