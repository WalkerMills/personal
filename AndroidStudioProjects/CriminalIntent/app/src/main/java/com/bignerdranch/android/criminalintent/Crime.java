package com.bignerdranch.android.criminalintent;

import java.util.UUID;

public class Crime {

    public UUID mId;
    public String mTitle;

    public Crime() {
        mId = UUID.randomUUID();
    }

    public UUID getId() {
        return mId;
    }

    public String getTitle() {
        return mTitle;
    }

    public void setTitle(String title) {
        mTitle = title;
    }
}