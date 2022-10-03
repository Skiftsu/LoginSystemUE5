package com.server;

import io.netty.bootstrap.ServerBootstrap;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelOption;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.nio.NioServerSocketChannel;
import io.netty.util.AttributeKey;

import com.server.Utils.Cryptography;

import org.hibernate.Session;
import org.hibernate.Transaction;

import javax.crypto.spec.SecretKeySpec;
import java.util.*;

public class ServerMain
{
    public final static AttributeKey<String> CHANNEL_OWNER = AttributeKey.valueOf("channel_owner");
    public final static AttributeKey<Session> HIBER_SESSION = AttributeKey.valueOf("hiber_session");
    public final static AttributeKey<Transaction> HIBER_TRANSACTION = AttributeKey.valueOf("hiber_transaction");
    public final static AttributeKey<Cryptography> CRYPTOGRAPHY = AttributeKey.valueOf("hiber_cryptography");

    private static final int Port = Integer.parseInt(ResourceBundle
            .getBundle("configuration", Locale.ENGLISH).getString("Port"));

    public static void main(String[] args)
    {
        EventLoopGroup bossGroup = new NioEventLoopGroup();
        EventLoopGroup workerGroup = new NioEventLoopGroup();

        try
        {
            ServerBootstrap b = new ServerBootstrap();
            b.group(bossGroup, workerGroup)
                    .channel(NioServerSocketChannel.class)
                    .childHandler(new ServerInitializer())
                    .childOption(ChannelOption.SO_KEEPALIVE, true)
                    .childOption(ChannelOption.TCP_NODELAY, true);

            ChannelFuture f = b.bind(Port).sync();
            f.channel().closeFuture().sync();
        }
        catch (InterruptedException e) {e.printStackTrace();}
        finally
        {
            workerGroup.shutdownGracefully();
            bossGroup.shutdownGracefully();
        }
    }
}