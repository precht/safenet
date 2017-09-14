package pl.safenet.server;

import com.sun.jersey.core.util.Base64;
import com.sun.jersey.spi.container.ContainerRequest;
import com.sun.jersey.spi.container.ContainerRequestFilter;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.HttpHeaders;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;


public class AuthFilter implements ContainerRequestFilter {

        // Exception thrown if user is unauthorized.
    private final static WebApplicationException unauthorized =
       new WebApplicationException(
           Response.status(Status.UNAUTHORIZED)
                   .header(HttpHeaders.WWW_AUTHENTICATE, "Basic realm=\"realm\"")
                   .entity("Page requires login.").build());
    
    @Override
    public ContainerRequest filter(ContainerRequest cr) throws WebApplicationException{
        
        //GET, POST, PUT ...
        String method = cr.getMethod();
        String path = cr.getPath(true);
             if (method.equals("GET") && path.equals("application.wadl"))
            return cr;
        
        // Get the authentication passed in HTTP headers parameters
        String auth = cr.getHeaderValue("authorization");
        if (auth == null)
            throw unauthorized;

        
        auth = auth.replaceFirst("[Bb]asic ", "");
        String userColonPass = Base64.base64Decode(auth);

        if (!userColonPass.equals("test:test"))
            throw unauthorized;

        return cr;
    }
    
}
