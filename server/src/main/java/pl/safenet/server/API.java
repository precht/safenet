package pl.safenet.server;
 
import com.sun.jersey.core.header.FormDataContentDisposition;
import com.sun.jersey.multipart.FormDataParam;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Consumes;
import javax.ws.rs.DELETE;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.ResponseBuilder;
 

@Path("api")
 
public class API {
 
    private static final String UPLOAD_FILE_SERVER = "C:/ProgramData/SafenetServer/";

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
 
    // DOWNLOADING KEY FROM SERVER
    @GET
    @Path("/listfiles")
    @Produces(MediaType.APPLICATION_JSON)
    public FileList listFiles() {
       
        System.out.println("Listing files in server directory...");
        FileList listToSend = new FileList();
 
        File folder = new File(UPLOAD_FILE_SERVER);
        File[] listOfFiles = folder.listFiles(new ImageFileFilter());
        List<String> fileList = new ArrayList<>();
 
        for (File aFile : listOfFiles) {
            if (aFile.isFile()) {
                System.out.println("File " + aFile.getName());
                fileList.add(aFile.getName());
            }
        }
 
        listToSend.setFileList(fileList);
        return listToSend;
    }
 
    @DELETE
    @Path("/delete")
    public Response deleteFiles() {
        System.out.println("Request for file deletion...");
        File folder = new File(".");
        File[] listOfFiles = folder.listFiles(new ImageFileFilter());
 
        boolean result = false;
 
        for (File aFile : listOfFiles) {
            if (aFile.isFile()) {
                System.out.println("File " + aFile.getName() + " was deleted");
                result = aFile.delete();
            }
        }
       
        if (result) {
            System.out.println("Files have been deleted");
            return Response.ok("File deleted successfully").build();
        } else {
            System.out.println("File was not deleted, unknown reason");
        }
            return Response.status (Response.Status.NOT_FOUND).build();
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
    @GET
        //@Path("/download/image")
        @Path("/download/image/{filename}")
        @Produces({"image/png", "image/jpg", "image/gif", "image/bmp"})
        public Response downloadImageFile(@PathParam("filename") String fileName) {
 
 
        String path = UPLOAD_FILE_SERVER + fileName;
                System.out.println("File to download " + path);
        File file = new File(path);
 
 
        if (file.exists()) {
            ResponseBuilder responseBuilder = Response.ok((Object) file);
            responseBuilder.header("Content-Disposition", "attachment; filename=\"" + fileName + "\"");
            return responseBuilder.build();
        } else {
            System.out.println("Could not find file specified.");
            return Response.status(Response.Status.NOT_FOUND).build();
        }
    }
 
    //UPLOADING IMAGE TO SERVER
    //uploading to user's working directory
    @POST
        @Path("/upload/image")
        @Consumes(MediaType.MULTIPART_FORM_DATA)
        public Response uploadImageFile(
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