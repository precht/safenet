/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package pl.ania.jerseyembeddedhttpserver;

import com.sun.jersey.api.core.PackagesResourceConfig;
import com.sun.jersey.api.core.ResourceConfig;
import com.sun.net.httpserver.HttpsServer;
import java.io.IOException;
import java.net.InetAddress;
import java.net.URI;
import java.net.UnknownHostException;
import java.security.KeyManagementException;
import java.security.NoSuchAlgorithmException;
import javax.ws.rs.core.UriBuilder;

/**
 *
 * @author ania
 */
public class JerseyEmbeddedHTTPServer {

    
    public static void main(String[] args) throws IOException, NoSuchAlgorithmException, KeyManagementException {
        System.out.println("Starting Embedded Jersey HTTPServer...\n");
        HttpsServer hTTPsServer = createHttpServer();
        
        
        System.out.println("Working Directory = " +
              System.getProperty("user.dir"));
        
        //hTTPsServer.start();
        SimpleHttpsServer simpleServer = new SimpleHttpsServer(hTTPsServer);
        simpleServer.Start();
        System.out.println(String.format("\nJersey Application Server started with WADL available at " + "%sapplication.wadl\n", getURI()));
        System.out.println("Started Embedded Jersey HTTPServer Successfully !!!");
    }

    private static HttpsServer createHttpServer() throws IOException {
        ResourceConfig resourceConfig = new PackagesResourceConfig("pl.ania.jerseyembeddedhttpserver");
        resourceConfig.getContainerResponseFilters().add(CORSFilter.class);
        return SecureServerFactory.create(getURI(), resourceConfig);
        //return HttpServerFactory.create(getURI(), resourceConfig);
    }

    private static URI getURI() {
        return UriBuilder.fromUri("https://" + getHostName() + "/").port(8085).build();
    }

    private static String getHostName() {
        String hostName = "localhost";
        try {
            hostName = InetAddress.getLocalHost().getCanonicalHostName();
        } catch (UnknownHostException e) {
            System.out.println(e);
        }
        return hostName;
    }
}
