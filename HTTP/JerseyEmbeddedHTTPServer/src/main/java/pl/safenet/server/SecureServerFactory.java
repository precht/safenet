package pl.safenet.server;

import com.sun.jersey.api.container.ContainerFactory;
import com.sun.jersey.api.core.ResourceConfig;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpsServer;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.URI;
import java.util.concurrent.Executors;


public class SecureServerFactory {

    //private static final int port = 8085;
    
    public static HttpsServer create(URI u, ResourceConfig rc)
            throws IOException, IllegalArgumentException {
        return create(u, ContainerFactory.createContainer(HttpHandler.class, rc));

    }

    public static HttpsServer create(URI u, HttpHandler handler)
            throws IOException, IllegalArgumentException {
        if (u == null) {
            throw new IllegalArgumentException("The URI must not be null");
        }

        final String scheme = u.getScheme();

        if (!scheme.equalsIgnoreCase("http") && !scheme.equalsIgnoreCase("https")) {
            throw new IllegalArgumentException("The URI scheme, of the URI " + u
                    + ", must be equal (ignoring case) to 'http' or 'https'");
        }

        final String path = u.getPath();

        if (path == null) {
            throw new IllegalArgumentException("The URI path, of the URI " + u
                    + ", must be non-null");
        } else if (path.length() == 0) {
            throw new IllegalArgumentException("The URI path, of the URI " + u
                    + ", must be present");
        } else if (path.charAt(0) != '/') {
            throw new IllegalArgumentException("The URI path, of the URI " + u
                    + ". must start with a '/'");
        }

        final int port = (u.getPort() == -1) ? 80 : u.getPort();

        final HttpsServer server = (scheme.equalsIgnoreCase("https"))
                ? HttpsServer.create(new InetSocketAddress(port), 0)
                : HttpsServer.create(new InetSocketAddress(port), 0);

        server.setExecutor(Executors.newCachedThreadPool());

        server.createContext(path, handler);

        return server;

    }
}
