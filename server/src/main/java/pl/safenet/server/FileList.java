package pl.safenet.server;

import java.util.List;
import javax.xml.bind.annotation.XmlRootElement;


@XmlRootElement
public class FileList {

    private List<String> fileList;

    public FileList() {

    }

    public FileList(List<String> fileList) {
        this.fileList = fileList;
    }

    public void setFileList(List<String> fileList) {
        this.fileList = fileList;
    }

    public List<String> getFileList() {
        return this.fileList;
    }
}
