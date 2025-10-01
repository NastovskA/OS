//Во класата бинарно пребарувачко дрво да се напише функција која за дадено дрво ќе го
//пронајде и врати хиерархиски најнискиот јазол кој е предок на јазлите со вредности n1 и n2, при
//што функцијата како аргументи прима корен на дрвото и вредностите на двата јазли.
//Да се напише главна програма во која ќе тестира работата на функцијата.

package ispit_3_7_2023_zadaca_1;

import java.util.*;

class BNode<E extends Comparable<E>> {
    public E info;
    public BNode<E> left, right;

    public BNode(E info) {
        this.info = info;
        this.left = null;
        this.right = null;
    }

    public BNode(E info, BNode<E> left, BNode<E> right) {
        this.info = info;
        this.left = left;
        this.right = right;
    }
}

class BSTree<E extends Comparable<E>> {
    public BNode<E> root;

    public BSTree() {
        root = null;
    }

    public BSTree(E info) {
        root = new BNode(info);
    }

    public void inorder(BNode<E> r) {
        if (r != null) {
            inorder(r.left);
            System.out.print(r.info + ", ");
            inorder(r.right);
        }
    }

    public BNode<E> insert(E info, BNode<E> r) {
        if (r == null) {
            return new BNode(info);
        }

        int comp = info.compareTo(r.info);
        if (comp < 0) { // left
            r.left = insert(info, r.left);
        } else if (comp > 0) { // right
            r.right = insert(info, r.right);
        } else {
            // ne pravi nishto
        }

        return r;
    }

    public boolean contains(E info, BNode<E> r) {
        if (r == null) {
            return false;
        }

        int comp = info.compareTo(r.info);
        if (comp < 0) {
            return contains(info, r.left);
        } else if (comp > 0) {
            return contains(info, r.right);
        } else {
            return true;
        }
    }

    public BNode<E> remove(E info, BNode<E> r) {
        if (r == null) {
            return r;
        }

        int comp = info.compareTo(r.info);
        if (comp < 0) {
            r.left = remove(info, r.left);
        } else if (comp > 0) {
            r.right = remove(info, r.right);
        } else { // brishi go jazolot info
            if (r.left != null && r.right != null) {
                r.info = findMin(r.right).info;
                r.right = remove(r.info, r.right);
            } else {
                if (r.left != null) {
                    return r.left;
                } else if (r.right != null) {
                    return r.right;
                } else {
                    return null;
                }
            }
        }

        return r;
    }

    private BNode<E> findMin(BNode<E> r) {
        if (r == null) {
            return null;
        } else if (r.left == null) {
            return r;
        } else {
            return findMin(r.left);
        }
    }

    public BNode<E> funkcija(BNode<E> r, E v1, E v2) {

        if (r == null) {
            return null;
        }

        if ((r.info.compareTo(v1) == 0) || (r.info.compareTo(v2) == 0)) {
            return r;
        } // AKO SAMIOT KOREN E EDEN OD JAZLITE TOGASH GO VRAKAM NEGO KAKO SIGURNO
          // ZAENICKO

        BNode<E> levastrana = funkcija(r.left, v1, v2);
        BNode<E> desnastrana = funkcija(r.right, v1, v2);

        if (levastrana != null && desnastrana != null) {
            return r;
        } // AKO V1 I V2 SE VO RALICNITE DELOVI O DRVOTO EDINSTVENO NESO STO GI SPOJUVA E
          // ROOT

        if (levastrana != null) {
            return levastrana;
        } else {

            return desnastrana;
        }

    }
}

public class ISPIT_3_7_2023_ZADACA_1 {

    public static void main(String[] args) {

        BSTree<Integer> tree = new BSTree<>(10);
        tree.insert(6, tree.root);
        tree.insert(12, tree.root);
        tree.insert(5, tree.root);
        tree.insert(7, tree.root);
        tree.insert(11, tree.root);
        tree.insert(3, tree.root);

        BNode<Integer> rez = tree.funkcija(tree.root, 37, 20);
        if (rez != null) {
            System.out.println("Najniskiot zaednicki poredok e: " + rez.info);
        } else {
            System.out.println("Ne postojat jazlite vo drvoto!");
        }

    }

}
