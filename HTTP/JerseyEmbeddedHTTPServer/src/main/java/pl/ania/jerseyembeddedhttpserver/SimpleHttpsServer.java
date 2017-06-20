/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package pl.ania.jerseyembeddedhttpserver;

import com.sun.net.httpserver.BasicAuthenticator;
import com.sun.net.httpserver.HttpsConfigurator;
import com.sun.net.httpserver.HttpsParameters;
import com.sun.net.httpserver.HttpsServer;
import java.io.FileInputStream;
import java.io.IOException;
import java.security.KeyManagementException;
import java.security.KeyStore;
import java.security.KeyStoreException;
import java.security.NoSuchAlgorithmException;
import java.security.UnrecoverableKeyException;
import java.security.cert.Certificate;
import java.security.cert.CertificateException;
import javax.net.ssl.KeyManagerFactory;
import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLEngine;
import javax.net.ssl.SSLParameters;
import javax.net.ssl.TrustManagerFactory;

/**
 *
 * @author ania
 */
public class SimpleHttpsServer {

    private final HttpsServer server;
    private static final String PROTOCOL = "TLS";

    public SimpleHttpsServer(HttpsServer aServer) {
        this.server = aServer;

    /*    server.setAuthenticator(new BasicAuthenticator("get") {
            @Override
            public boolean checkCredentials(String user, String pwd) {
                return user.equals("admin") && pwd.equals("password");
            }
        });*/
    }

    public void Start() throws KeyManagementException {
        try {
            // load certificate
            String keystoreFilename = getPath() + "mycert.keystore";
            char[] storepass = "mypassword".toCharArray();
            char[] keypass = "mypassword".toCharArray();
            String alias = "selfsigned";
            FileInputStream fIn = new FileInputStream(keystoreFilename);
            KeyStore keystore = KeyStore.getInstance("JKS");
            keystore.load(fIn, storepass);
            // display certificate
		//Certificate cert = keystore.getCertificate(alias);
		//System.out.println(cert);
                        
            // setup the key manager factory
            KeyManagerFactory kmf = KeyManagerFactory.getInstance("SunX509");
            kmf.init(keystore, keypass);

            // setup the trust manager factory
            TrustManagerFactory tmf = TrustManagerFactory.getInstance("SunX509");
            tmf.init(keystore);

            // create ssl context
            SSLContext sslContext = SSLContext.getInstance(PROTOCOL);
            // setup the HTTPS context and parameters
            sslContext.init(kmf.getKeyManagers(), tmf.getTrustManagers(), null);
            server.setHttpsConfigurator(new HttpsConfigurator(sslContext) {
                @Override
                public void configure(HttpsParameters params) {
                    try {
                        // initialise the SSL context
                        SSLContext c = SSLContext.getDefault();
                        SSLEngine engine = c.createSSLEngine();
                        //params.setNeedClientAuth(false);
                        params.setNeedClientAuth(true);
                        params.setCipherSuites(engine.getEnabledCipherSuites());
                        params.setProtocols(engine.getEnabledProtocols());

                        // get the default parameters
                        SSLParameters defaultSSLParameters = c.getDefaultSSLParameters();
                        params.setSSLParameters(defaultSSLParameters);
                    } catch (NoSuchAlgorithmException ex) {
                        System.out.println("Failed to create HTTPS server");
                    }
                }
            });

            System.out.println("server started...");
            server.setExecutor(null);
            server.start();
        } catch (IOException | NoSuchAlgorithmException | KeyStoreException | CertificateException | UnrecoverableKeyException e) {
            System.out.println(e);
        }

    }

    private String getPath() {
        return System.getProperty("user.dir") + "/";
    }

    public void Stop() {
        server.stop(0);
        System.out.println("server stopped");
    }
}
