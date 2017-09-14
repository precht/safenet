package pl.safenet.server;

import com.sun.jersey.api.core.PackagesResourceConfig;
import com.sun.jersey.api.core.ResourceConfig;
import java.io.IOException;
import java.net.URI;
import java.security.KeyManagementException;
import java.security.NoSuchAlgorithmException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.ws.rs.core.UriBuilder;


public class SafenetServer {

    private static final SimpleHttpsServer SAFENET_SERVER = createSafenetServer();
    
    public static void start(String[] args) {
        System.out.println("Starting Safenet Server.");
        try {
            SAFENET_SERVER.Start();
        } catch (KeyManagementException ex) {
            Logger.getLogger(SafenetServer.class.getName()).log(Level.SEVERE, null, ex);
        }
        System.out.println("Safenet Server started.");
    }
 
    public static void stop(String[] args) {
        System.out.println("Stoping Safenet Server.");
        SAFENET_SERVER.Stop();
        System.out.println("Safenet Server stopped.");
    }

    private static SimpleHttpsServer createSafenetServer() {
        ResourceConfig resourceConfig = new PackagesResourceConfig("pl.safenet.server");
        resourceConfig.getContainerResponseFilters().add(CORSFilter.class);
        resourceConfig.getProperties().put(
                "com.sun.jersey.spi.container.ContainerRequestFilters",
                "pl.safenet.server.AuthFilter");
        
        SimpleHttpsServer server = null;
        try {
            server = new SimpleHttpsServer(SecureServerFactory.create(getURI(), resourceConfig));
        } catch (IOException | IllegalArgumentException ex) {
            Logger.getLogger(SafenetServer.class.getName()).log(Level.SEVERE, null, ex);
        }
        return server;
    }

    private static URI getURI() {
        return UriBuilder.fromUri("https://" + getHostName() + "/").port(8085).build();
    }

    private static String getHostName() {
        String hostName = "localhost";
        return hostName;
    }

    public static void main(String[] args) throws IOException, NoSuchAlgorithmException, KeyManagementException {
        System.out.println("Starting Embedded Jersey HTTPServer...\n");
        System.out.println("Working Directory = C:/ProgramData/SafenetServer");
        SAFENET_SERVER.Start();
        System.out.println(String.format("\nJersey Application Server started with WADL available at " + "%sapplication.wadl\n", getURI()));
        System.out.println("Successfully Started Safenet Server!!!");
    }
}

