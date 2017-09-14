package pl.safenet.server;

import com.sun.jersey.api.core.PackagesResourceConfig;
import com.sun.jersey.api.core.ResourceConfig;
import com.sun.net.httpserver.HttpsServer;
import java.io.IOException;
import java.net.URI;
import java.security.KeyManagementException;
import java.security.NoSuchAlgorithmException;
import javax.ws.rs.core.UriBuilder;


public class SafenetServer {

    public static void main(String[] args) throws IOException, NoSuchAlgorithmException, KeyManagementException {
        System.out.println("Starting Embedded Jersey HTTPServer...\n");
        HttpsServer hTTPsServer = createHttpServer();

        System.out.println("Working Directory = "
                + System.getProperty("user.home") + "/.safenet/");

        SimpleHttpsServer simpleServer = new SimpleHttpsServer(hTTPsServer);
        simpleServer.Start();
        System.out.println(String.format("\nJersey Application Server started with WADL available at " + "%sapplication.wadl\n", getURI()));
        System.out.println("Successfully Started Safenet Server!!!");
    }

    private static HttpsServer createHttpServer() throws IOException {
        ResourceConfig resourceConfig = new PackagesResourceConfig("pl.safenet.server");
        resourceConfig.getContainerResponseFilters().add(CORSFilter.class);
        resourceConfig.getProperties().put(
                "com.sun.jersey.spi.container.ContainerRequestFilters",
                "pl.safenet.server.AuthFilter");
        
        return SecureServerFactory.create(getURI(), resourceConfig);
    }

    private static URI getURI() {
        return UriBuilder.fromUri("https://" + getHostName() + "/").port(8085).build();
    }

    private static String getHostName() {
        String hostName = "localhost";
        return hostName;
    }
}
