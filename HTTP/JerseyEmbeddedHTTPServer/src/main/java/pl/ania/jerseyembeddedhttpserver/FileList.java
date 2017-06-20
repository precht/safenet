/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package pl.ania.jerseyembeddedhttpserver;

import java.util.List;
import javax.xml.bind.annotation.XmlRootElement;

/**
 *
 * @author ania
 */

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
