/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package pl.ania.jerseyembeddedhttpserver;

import javax.xml.bind.annotation.XmlRootElement;

/**
 *
 * @author ania
 */
@XmlRootElement
public class Key {

    private double x, y, a, H;
    private int G1, G2;

    public Key() {
        this.x = 0.0;
        this.y = 0.0;
        this.a = 0.0;
        this.H = 0.0;
        this.G1 = 0;
        this.G2 = 0;
    }

    public void setX(double x) {
        this.x = x;
    }

    public void setY(double y) {
        this.y = y;
    }

    public void setA(double a) {
        this.a = a;
    }

    public void setH(double H) {
        this.H = H;
    }

    public void setG1(int G1) {
        this.G1 = G1;
    }

    public void setG2(int G2) {
        this.G2 = G2;
    }

    public double getX() {
        return x;
    }

    public double getY() {
        return y;
    }

    public double getA() {
        return a;
    }

    public double getH() {
        return H;
    }

    public int getG1() {
        return G1;
    }

    public int getG2() {
        return G2;
    }
}
