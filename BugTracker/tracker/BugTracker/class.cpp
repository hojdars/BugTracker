/* Class define */

class my_widget
{
    virtual ~my_widget() = default;
    virtual QString get_text() = 0;
}

class my_ComboBox : public my_widget_ptr
{
private:
    QComboBox * ptr_;
public:
    virtual QString get_text()
    {
        return ptr_->getCurrent();
    }
}

class my_lineEdit : public my_widget
{
private:
    QLineEdit * ptr_;
public:
    virtual QString get_text()
    {
        return ptr_->text();
    }
}

/* Item Edit Dialog */
std::vector<my_widget *> vec_;

/* return texts */
for(const auto & i : vec_)
{
    i->get_text();
}
