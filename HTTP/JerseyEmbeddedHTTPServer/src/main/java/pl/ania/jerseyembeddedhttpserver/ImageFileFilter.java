/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package pl.ania.jerseyembeddedhttpserver;

import java.io.*;

/**
 *
/**
 * A class that implements the Java FileFilter interface.
 */
public class ImageFileFilter implements FileFilter
{
  private final String[] acceptedExtensions = new String[] {".bmp", ".png", ".jpg", ".jpeg"};

  @Override
  public boolean accept(File file)
  {
    for (String extension : acceptedExtensions)
    {
      if (file.getName().toLowerCase().endsWith(extension))
      {
        return true;
      }
    }
    return false;
  }
}
