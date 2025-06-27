package com.example.lucene;

import java.util.List;

public class Book {
    private String title;
    private String description;
    private String author;
    private String publisher;
    private String publishYear;
    private String isbn;
    private List<String> tagsArray;
    private String price;
    private String coverImage;
    private String listedTime;
    private String sellerID;

    public String getTitle() { return title; }
    public void setTitle(String title) { this.title = title; }

    public String getDescription() { return description; }
    public void setDescription(String content) { this.description = content; }

    public String getAuthor() { return author; }
    public void setAuthor(String author) { this.author = author; }

    public String getPublisher() { return publisher; }
    public void setPublisher(String publisher) { this.publisher = publisher; }

    public String getPublishYear() { return publishYear; }
    public void setPublishYear(String publishYear) { this.publishYear = publishYear; }

    public String getIsbn() { return isbn; }
    public void setIsbn(String isbn) { this.isbn = isbn; }

    public List<String> getTagsArray() { return tagsArray; }
    public void setTagsArray(List<String> tagsArray) { this.tagsArray = tagsArray; }

    public String getPrice() { return price; }
    public void setPrice(String price) { this.price = price; }

    public String getCoverImage() { return coverImage; }
    public void setCoverImage(String coverImage) { this.coverImage = coverImage; }

    public String getListedTime() { return listedTime; }
    public void setListedTime(String listedTime) { this.listedTime = listedTime; }

    public String getSellerID() { return sellerID; }
    public void setSellerID(String sellerID) { this.sellerID = sellerID; }
}
