package com.server.Handlers.Logics;

import com.server.Protobufs.KeyMessages;
import com.server.Protobufs.LoginMessages;
import io.netty.channel.ChannelHandlerContext;
import org.hibernate.Session;
import org.hibernate.Query;

import java.util.List;

import static com.server.ServerMain.HIBER_SESSION;

public class CheckRegParameter
{
    public CheckRegParameter(ChannelHandlerContext ctx, KeyMessages.Wrapper wrapper)
    {
        Session session = ctx.channel().attr(HIBER_SESSION).get();

        LoginMessages.CheckRegParameterStatus CheckRegParameterStatus;
        if(wrapper.getCheckRegParameterStatus().hasLogin())
        {
            String Login = wrapper.getCheckRegParameterStatus().getLogin();
            if(Login.isEmpty()) return;
            Query query = session.createQuery("SELECT login FROM UserModel WHERE login = :str ");
            List users = query.setParameter("str", Login).list();
            CheckRegParameterStatus = LoginMessages.CheckRegParameterStatus.newBuilder().setLoginStatus(users.isEmpty()).build();
        }
        else if(wrapper.getCheckRegParameterStatus().hasMail())
        {
            String Mail = wrapper.getCheckRegParameterStatus().getMail();
            if(Mail.isEmpty()) return;
            Query query = session.createQuery("SELECT mail FROM UserModel WHERE mail = :str ");
            List mails = query.setParameter("str", Mail).list();
            CheckRegParameterStatus = LoginMessages.CheckRegParameterStatus.newBuilder().setMailStatus(mails.isEmpty()).build();
        }
        else return;


        ctx.writeAndFlush(KeyMessages.Wrapper.newBuilder().setCheckRegParameterStatus(CheckRegParameterStatus).build());
    }
}