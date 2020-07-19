import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, debug, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


@app.after_request
def after_request(response):
    # Ensure responses aren't cached
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    stocks = []
    total = 0

    stocks_rows = db.execute(
        "SELECT * FROM 'stocks' WHERE user_id = ?", session["user_id"])
    if len(stocks_rows) > 0:
        for stock in stocks_rows:
            symbol = stock["symbol"]
            shares = stock["shares"]
            quote = lookup(symbol)
            name = quote["name"] if quote else '-'
            price = quote["price"] if quote else '-'
            stock_total = quote["price"] * stock["shares"] if quote else '-'
            stocks.append({'symbol': symbol, 'name': name,
                           'shares': shares, 'price': usd(price), 'total': usd(stock_total)})
            total += stock_total

    # get user cash
    user_rows = db.execute(
        "SELECT * FROM 'users' WHERE id = ?", session["user_id"])
    if len(user_rows) != 1:
        return apology("Something broke :(", 500)

    cash = user_rows[0]["cash"]
    stocks.append({'symbol': 'CASH', 'name': '-', 'shares': '-',
                   'price': '-', 'total': usd(cash)})
    total += cash

    return render_template("index.html", stocks=stocks, total=usd(total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "GET":
        return render_template("buy.html")

    # both shares and symbol are set as required in the template - can't be empty
    shares = int(request.form.get("shares"))
    if shares <= 0:
        return apology("Invalid number of shares", 422)

    quote = lookup(request.form.get("symbol"))
    if not quote:
        return apology("This stock doesn't exist", 500)

    # calculate if user can purchase
    rows = db.execute("SELECT cash FROM users WHERE id = :id",
                      id=session["user_id"])
    if len(rows) != 1:
        return apology("Error while process the request", 500)

    available_cash = rows[0]["cash"]
    total_price = quote["price"] * shares
    if available_cash == 0 or total_price > available_cash:
        return apology("You don't have enough money.")

    # store the transaction
    purchase_result = db.execute("INSERT INTO 'purchases' (value, date, symbol, shares, user_id) VALUES (:value, datetime('now'), :symbol, :shares, :user_id)",
                                 value=total_price * -1, symbol=quote["symbol"], shares=shares, user_id=session["user_id"])
    if not purchase_result:
        return apology("Something broke :(", 500)

    # update user cash
    new_balance = round(available_cash - total_price, 2)
    cash_update_result = db.execute(
        "UPDATE 'users' SET cash = :cash WHERE id = :id", cash=new_balance, id=session["user_id"])
    if not cash_update_result:
        return apology("Something broke :(", 500)

    # update user stocks
    user_stocks = db.execute(
        "SELECT * FROM 'stocks' WHERE user_id = :user_id AND symbol = :symbol", user_id=session["user_id"], symbol=quote["symbol"])
    if len(user_stocks) == 1:
        new_shares = user_stocks[0]["shares"] + shares
        update_result = db.execute(
            "UPDATE 'stocks' SET shares = :shares WHERE user_id = :user_id", shares=new_shares, user_id=session["user_id"])
        if not update_result:
            return apology("Something broke :(", 500)
    else:
        insertion_result = db.execute("INSERT INTO 'stocks' (symbol, shares, user_id) VALUES (:symbol, :shares, :user_id)",
                                      symbol=quote["symbol"], shares=shares, user_id=session["user_id"])
        if not insertion_result:
            return apology("Something broke :(", 500)

    flash("Transaction completed")
    return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    return apology("TODO")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "GET":
        return render_template("quote.html")

    quote = lookup(request.form.get("symbol"))

    if not quote:
        return apology("Try again with a valid stock :c", 500)

    return render_template("quoted.html", quote=quote)


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "GET":
        return render_template("register.html")

    # (basic) password validation is handled through HTML
    password = request.form.get("password")
    password_confirmation = request.form.get("password-confirmation")

    if password != password_confirmation:
        return apology("The passwords don't match", 400)

    username = request.form.get("username")

    # check doesn't exist in DB
    rows = db.execute("SELECT * FROM users WHERE username = :username",
                      username=username)
    if len(rows) != 0:
        return apology("invalid username", 403)

    result = db.execute("INSERT INTO users (username, hash) VALUES (:username, :password)",
                        username=username, password=generate_password_hash(password))
    if not result:
        return apology("something went wrong, try again", 500)

    return redirect("/login")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    return apology("TODO")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
