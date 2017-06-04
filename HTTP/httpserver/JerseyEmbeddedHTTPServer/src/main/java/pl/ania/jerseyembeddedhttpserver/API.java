/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package pl.ania.jerseyembeddedhttpserver;

import com.sun.jersey.core.header.FormDataContentDisposition;
import com.sun.jersey.multipart.FormDataParam;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Consumes;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.ResponseBuilder;

/**
 *
 * @author ania
 */
@Path("api")
public class API {

    private static final String UPLOAD_FILE_SERVER = System.getProperty("user.dir") + "/";
    private Key key;

    // DOWNLOADING KEY FROM SERVER
    @GET
    @Path("/download/key")
    @Produces(MediaType.APPLICATION_JSON)
    public Key downloadKey() {
        Key aKey = new Key();
        aKey.setX(1.25);
        aKey.setY(1.25);
        aKey.setA(1.25);
        aKey.setH(1.25);
        aKey.setG1(1);
        aKey.setG2(1);
        key = aKey;
        return aKey;
    }

    // UPLOADING KEY TO SERVER
    @POST
    @Consumes({MediaType.APPLICATION_JSON})
    @Produces({MediaType.TEXT_PLAIN})
    @Path("/upload/key")
    public String uploadKey(Key key) throws Exception {
        
        System.out.println(key.getX());
        System.out.println(key.getY());
        System.out.println(key.getA());
        System.out.println(key.getH());
        System.out.println(key.getG1());
        System.out.println(key.getG2());

        return "ok";
    }

    //DOWNLOADING IMAGE FROM SERVER
    //File - lena.png in user's home directory
    @GET
    @Path("/download/image")
    @Produces({"image/png", "image/jpg", "image/gif", "image/bmp"})
    public Response downloadImageFile() {

        // set file (and path) to be downloaded
        String path = System.getProperty("user.dir") + "/senttoserver.png";
        File file = new File(path);

        if (file.exists()) {
            ResponseBuilder responseBuilder = Response.ok((Object) file);
            responseBuilder.header("Content-Disposition", "attachment; filename=\"lena.png\"");
            return responseBuilder.build();
        } else {
            return Response.status(Response.Status.NOT_FOUND).build();
        }

    }

    //UPLOADING IMAGE TO SERVER
    //uploading to user's working directory
    @POST
    @Path("/upload/image")
    @Consumes(MediaType.MULTIPART_FORM_DATA)
    public Response uploadTextFile(
            @FormDataParam("file") InputStream fileInputStream,
            @FormDataParam("file") FormDataContentDisposition fileFormDataContentDisposition
    ) {
        String fileName = null;
        String uploadFilePath = null;

        try {
            fileName = fileFormDataContentDisposition.getFileName();
            System.out.println("File name: " + fileName);
            uploadFilePath = writeToFileServer(fileInputStream, fileName);
        } catch (IOException ioe) {
            System.out.println(ioe);
        } finally {
            // release resources, if any
        }
        return Response.ok("File uploaded successfully at " + uploadFilePath).build();
    }

    /**
     * write input stream to file server
     *
     * @param inputStream
     * @param fileName
     * @throws IOException
     */
    private String writeToFileServer(InputStream inputStream, String fileName) throws IOException {

        OutputStream outputStream = null;
        String qualifiedUploadFilePath = UPLOAD_FILE_SERVER + fileName;

        try {
            outputStream = new FileOutputStream(new File(qualifiedUploadFilePath));
            int read = 0;
            byte[] bytes = new byte[1024];
            while ((read = inputStream.read(bytes)) != -1) {
                outputStream.write(bytes, 0, read);
            }
            outputStream.flush();
        } catch (IOException ioe) {
            System.out.println(ioe);
        } finally {
            //release resource, if any
            outputStream.close();
        }
        return qualifiedUploadFilePath;
    }
}
